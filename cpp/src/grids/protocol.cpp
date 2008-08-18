#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL_net.h>

#include <json/writer.h>
#include <json/reader.h>
#include <json/value.h>

#include <grids/define.h>
#include <grids/protocol.h>

namespace Grids {
  void Protocol::setEventCallback(gevent_callback_t cb, void *userData) { eventCallback = cb; eventCallbackUserData = userData; }
  void Protocol::setConnectedCallback(gevent_callback_t cb, void *userData) { connectedCallback = cb; connectedCallbackUserData = userData; }

  int runEventLoopThreadEntryPoint(void *arg) {
    Protocol *gp = (Protocol *)arg;
    gp->runEventLoop();
	return 0;
  }
  /*
  void *dispatchEventEntryPoint(void *arg) {
    Protocol *gp = (Protocol *)arg;
    gp->dispatchEvent();
  }
  */
  Protocol::Protocol() {
    sock = 0;
    finishedMutex = SDL_CreateMutex();
    running = 0;
    connectedCallback = NULL;
  }

  Protocol::~Protocol() {
      SDL_DestroyMutex(finishedMutex);
  }

  bool Protocol::connectToNode(const char *address) {
    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, (char *)address, GRIDS_PORT) == -1) {
      printf("Could not resolve hostname %s: %s\n", address, SDLNet_GetError());
      return 0;
    }

    sock = SDLNet_TCP_Open(&ip);
    if (! sock) {
      printf("Failed to connect to host %s: %s\n", address, SDLNet_GetError());
      return 0;
    }

    // hooray we are connnected! initialize protocol
    sendProtocolInitiationString();

    return 1;
  }

  void Protocol::sendProtocolInitiationString() {
    protocolWrite("==Grids/1.0/JSON");
  }

  int Protocol::protocolWrite(const char *str) {
    uint32_t len = strlen(str);

    unsigned int outstr_len = len + 4;
    char *outstr = (char *)malloc(outstr_len);

    SDLNet_Write32(len, outstr);
    memcpy((outstr + 4), str, len);

    int ret = SDLNet_TCP_Send(sock, outstr, outstr_len);
    //    free(outstr);

    if (ret != outstr_len) {
      printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
      // It may be good to disconnect sock because it is likely invalid now.
    }

    return ret;
  }

  void Protocol::closeConnection() {
    SDLNet_TCP_Close(sock);
  }

  std::string Protocol::stringifyMap(gridsmap_t *m) {
    Json::FastWriter *writer = new Json::FastWriter();
    Json::Value root = mapToJsonValue(m);
    return writer->write(root);
  }

  void Protocol::sendRequest(std::string evt) {
    sendRequest(evt, NULL);
  }

  void Protocol::sendRequest(std::string evt, gridsmap_t *args) {
    if (evt.empty())
      return;

    if (args == NULL) {
      args = new gridsmap_t();
    }

    const static std::string methodkey = "_method";
    (*args)[methodkey] = evt;
    std::string msg = stringifyMap(args);

    protocolWrite(msg.c_str());
  }

  Json::Value Protocol::mapToJsonValue(gridsmap_t *m) {
    giterator mapIterator;

    Json::Value jsonVal;

    for(mapIterator = m->begin();
        mapIterator != m->end();
        mapIterator++) {

      gridskey_t key = mapIterator->first;
      gridsval_t val = mapIterator->second;

      jsonVal[key] = val;
    }

    return jsonVal;
  }

  void Protocol::runEventLoop() {
    int bytesRead;
    uint32_t incomingLength;
    char *buf;
    char *bufIncoming;

    setFinished(0);

    while (! isFinished() && sock) {
      // read in 4 byte length of message
      bytesRead = SDLNet_TCP_Recv(sock, &incomingLength, 4);

      incomingLength = SDLNet_Read32(&incomingLength);

      if (bytesRead < 0) {
        std::cerr << "Socket read error: " << SDLNet_GetError() << "\n";
        continue;
      }

      if (bytesRead != 4) {
        // socket broken most likely
        std::cerr << "failed to read from socket\n";
        // break;
        continue;
      }

      if (incomingLength > 1024 * 1024 * 1024) {
        // not going to read in more than a gig, f that
        std::cerr << "Got incoming message size: " << incomingLength << ". Skipping\n";
        continue;
      }

      // TODO: run in seperate thread

      // allocate space for incoming message + null byte
      buf = (char *)malloc(incomingLength + 1);

      std::cout << "incoming: " << incomingLength << "\n";
      uint32_t bytesRemaining = incomingLength;
      bufIncoming = buf;

      do {
        bytesRead = SDLNet_TCP_Recv(sock, bufIncoming, bytesRemaining);

        if (bytesRead > 0) {
          bytesRemaining -= bytesRead;
          bufIncoming += bytesRead;
        }

      } while ((bytesRead > 0) && bytesRemaining && ! isFinished());
      buf[incomingLength] = '\0';

        if (bytesRead == -1) {
          // o snap read error
          std::cerr << "Socket read error: " << bytesRead << "\n";
          free(buf);
          break;
        }

        if (bytesRead == 0) {
          // not chill
          std::cerr << "Didn't read any data when expecting message of " << incomingLength << " bytes\n";
          free(buf);
          continue;
        }

        if (bytesRead != incomingLength) {
          std::cerr << "Only read " << bytesRead << " bytes when expecting message of "
                    << incomingLength << " bytes\n";
          free(buf);
          continue;
        }

        // TODO: run in seperate thread
        std::string msg = buf;
        handleMessage(msg);

        free(buf);
    }
  }

  void Protocol::handleMessage(std::string &msg) {
    if (msg.size() < 2) return; // obv. bogus

    if (msg.find("==", 0, 2) == 0) {
      // protocol initiation message

      if (connectedCallback)
        connectedCallback(this, NULL, connectedCallbackUserData);

    } else if (msg.find("--", 0, 2) == 0) {
      // encrypted protocol message
    } else {
      // assume this is json for now
      Json::Value root = parseJson(msg);

      // FIXME: this is slow and lame
      //gridsmap_t rootMap = jsonToMap(root);

      Event *evt = new Event(root["_method"].asString(), root);
      eventCallback(this, evt, eventCallbackUserData);
      delete evt;
    }
  }

  gridsmap_t Protocol::jsonToMap(Json::Value &root) {
    // ghetto, should fix in the future
    Json::Value::Members memberList = root.getMemberNames();
    std::vector<std::string>::iterator iter;

    gridsmap_t outMap;

    for (iter = memberList.begin(); iter != memberList.end(); iter++) {
      Json::Value val = root[*iter];
      outMap[*iter] = val;
    }

    return outMap;
  }


  Json::Value Protocol::parseJson(std::string &msg) {
    Json::Value root;
    Json::Reader reader;

    if (reader.parse(msg, root))
      return root;

    std::cerr << "Could not parse JSON: " << msg << "\n";
    return Json::Value(0);
  }

  int Protocol::runEventLoopThreaded() {
    SDL_mutexP(finishedMutex);

    eventLoopThread = SDL_CreateThread(runEventLoopThreadEntryPoint, this);

    threadsFinished[getThreadId()] = 0;
    SDL_mutexV(finishedMutex);

	return 0;
  }

  void Protocol::stopEventLoopThread() {
    SDL_mutexP(finishedMutex);
    setFinished(1);
    SDL_mutexV(finishedMutex);

    if (running) {
      SDL_WaitThread(eventLoopThread, NULL);
      running = 0;
    }
  }

  uint32_t Protocol::getThreadId() {
    return SDL_GetThreadID(eventLoopThread);
  }

  bool Protocol::isFinished() {
    bool isFinished;

    SDL_mutexP(finishedMutex);
    isFinished = threadsFinished[getThreadId()];
    SDL_mutexV(finishedMutex);

    return isFinished;
  }

  void Protocol::setFinished(bool fin) {
    uint32_t threadId = getThreadId();

    if (! threadId)
        return;

    SDL_mutexP(finishedMutex);
    threadsFinished[threadId] = fin;
    SDL_mutexV(finishedMutex);
  }

}
