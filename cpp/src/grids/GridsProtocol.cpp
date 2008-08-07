#include <iostream>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>

#include <json/writer.h>

#include <grids/GridsDefine.h>
#include <grids/GridsProtocol.h>

namespace Grids {
  void *runEventLoopThreadEntryPoint(void *arg) {
    Protocol *gp = (Protocol *)arg;
    gp->runEventLoop();
  }
  /*
  void *dispatchEventEntryPoint(void *arg) {
    Protocol *gp = (Protocol *)arg;
    gp->dispatchEvent();
  }
  */
  Protocol::Protocol() {
    sock = 0;
    finished = 0;
    pthread_mutex_init(&finishedMutex, NULL);
    eventLoopThread = (pthread_t)NULL;
  }

  bool Protocol::connectToNode(const char *address) {
    // look up host
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1;

    if ((hp = gethostbyname(address)) == NULL) {
      herror("gethostbyname");
      return 0;
    }

    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(GRIDS_PORT);
    addr.sin_family = AF_INET;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
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
    uint32_t len_net = htonl(len);

    unsigned int outstr_len = len + 4;
    char *outstr = (char *)malloc(outstr_len);

    memcpy(outstr, &len_net, 4);
    memcpy((outstr + 4), str, len);

    return write(sock, outstr, outstr_len);
  }

  void Protocol::closeConnection() {
    ::shutdown(sock, SHUT_RDWR); 
    close(sock); 
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

  void Protocol::setEventCallback(gevent_callback_t cb) { eventCallback = cb; }

  void Protocol::runEventLoop() {
    int bytesRead;
    uint32_t incomingLength;
    char *buf;
    char *bufIncoming;

    /*    buf = (char*)malloc(100000);
    read(sock, buf, sizeof(buf));
    read(sock, buf, sizeof(buf));
    std::cout << "buf: " << buf;
    */
    while (! finished && sock) {
      // read in 4 byte length of message
      bytesRead = read(sock, &incomingLength, 4);
      incomingLength = ntohl(incomingLength);

      //std::cout << "bytesRead: " << bytesRead << " incoming: " << incomingLength << "\n";
      
      if (bytesRead == -1) {
        // uh oh, socket read error
        std::cerr << "Socket read error: " << bytesRead << "\n";
        break;
      }

      if (bytesRead != 4) {
        // wtf? try reading again
        continue;
      }

      if (incomingLength > 1024 * 1024 * 1024) {
        // not going to read in more than a gig, f that
        std::cerr << "Got incoming message size: " << incomingLength << ". Skipping\n";
        continue;
      }

      // ok time to read some shit in
      buf = (char *)malloc(incomingLength);

      //std::cout << "incoming: " << incomingLength << "\n";
      uint32_t bytesRemaining = incomingLength;
      bufIncoming = buf;

      do {
        bytesRead = read(sock, bufIncoming, bytesRemaining);

        if (bytesRead > 0) {
          bytesRemaining -= bytesRead;
          bufIncoming += bytesRead;
        }

        //std::cout << "read: " << bytesRead << " remaining: " << bytesRemaining << "\n";

      } while ((bytesRead > 0 || bytesRead != EAGAIN) && bytesRemaining);
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

      std::cout << "Got message \"" << buf << "\"\n";
      
      free(buf);
    }

    std::cout << "ended read thread\n";
  }

  int Protocol::runEventLoopThreaded() {
    return pthread_create(&eventLoopThread, NULL, runEventLoopThreadEntryPoint, this);
  }

  void Protocol::stopEventLoopThread() {
    pthread_mutex_lock(&finishedMutex);
    finished = 1;
    pthread_mutex_unlock(&finishedMutex);

    if (eventLoopThread)
      pthread_join(eventLoopThread, NULL);
  }

}
