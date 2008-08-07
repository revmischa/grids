#include <iostream>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include <json/writer.h>

#include <GridsDefine.h>
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

    unsigned int outstr_len = len + 5;
    char sep = ',';
    char *outstr = (char *)malloc(outstr_len);

    memcpy(outstr, &len_net, 4);
    memcpy((outstr + 4), &sep, 1);
    memcpy((outstr + 5), str, len);

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
    int bytes_read;
    uint32_t incomingLength;
    char *buf;

    while (! finished && sock) {
      // read in 4 byte length of message
      bytes_read = read(sock, &incomingLength, 4);
      
      if (bytes_read == -1) {
        // uh oh, socket read error
        break;
      }

      if (bytes_read != 4) {
        // wtf? try reading again
        continue;
      }

      if (incomingLength > 1024 * 1024 * 1024) {
        // not going to read in more than a gig, f that
        continue;
      }

      // ok time to read some shit in
      bytes_read = read(sock, buf, incomingLength);

      if (bytes_read == -1) {
        // o snap read error
        break;
      }

      if (bytes_read = 0) {
        // not chill
        std::cerr << "Didn't read any data when expecting message of " << incomingLength << " bytes\n";
        continue;
      }

      if (bytes_read != incomingLength) {
        std::cerr << "Only read " << bytes_read << " bytes when expecting message of "
                  << incomingLength << " bytes\n";
        continue;
      }

      std::cout << "Got message \"" << buf << "\"\n";
    }
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
