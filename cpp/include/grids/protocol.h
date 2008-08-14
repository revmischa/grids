#ifndef GRIDS_PROTO_H
#define GRIDS_PROTO_H

#include <map>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>
#include <SDL_net.h>
#include <json/value.h>
#include <grids/event.h>
#include <grids/define.h>

namespace Grids {
  const unsigned int GRIDS_PORT = 1488;

  // keep track of which threads are done
  static std::map<int, bool> threadsFinished;

  class Protocol;
  typedef void (*gevent_callback_t)(Protocol *, Event *, void *userData);

  // class method
  int runEventLoopThreadEntryPoint(void *);

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
    void setEventCallback(gevent_callback_t, void *userData);
    void runEventLoop();
    int runEventLoopThreaded();
    void stopEventLoopThread();
    void handleMessage(std::string &msg);
    bool isFinished();
    void setFinished(bool);
    gridsmap_t jsonToMap(Json::Value &);
    Json::Value parseJson(std::string &msg);
    Uint32 getThreadId();

  private:
    Json::Value mapToJsonValue(gridsmap_t *);
    void dispatchEvent(Grids::Event *);
    TCPsocket sock;
    gevent_callback_t eventCallback;
    void *eventCallbackUserData;
    SDL_mutex *finishedMutex;
    SDL_Thread *eventLoopThread;
    bool running;
  };
}

#endif
