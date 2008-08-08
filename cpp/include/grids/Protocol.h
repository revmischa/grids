#pragma once

#include <map>
#include <string>
#include <pthread.h>
#include <json/value.h>
#include <grids/event.h>
#include <grids/define.h>

namespace Grids {
  void *runEventLoopThreadEntryPoint(void *);
  const unsigned int GRIDS_PORT = 1488;

  class Protocol;
  typedef void (*gevent_callback_t)(Protocol *, Event *);

  class Protocol {
  public:
    Protocol();
    bool connectToNode(const char *address);
    void sendProtocolInitiationString();
    int protocolWrite(const char *);
    std::string stringifyMap(gridsmap_t *m);
    void sendRequest(std::string);
    void sendRequest(std::string, gridsmap_t *args);
    void closeConnection();
    void setEventCallback(gevent_callback_t);
    void runEventLoop();
    int runEventLoopThreaded();
    void stopEventLoopThread();

  private:
    Json::Value mapToJsonValue(gridsmap_t *);
    void dispatchEvent(Grids::Event *);
    int sock;
    gevent_callback_t eventCallback;
    pthread_mutex_t finishedMutex;
    short finished;
    pthread_t eventLoopThread;
  };
}

