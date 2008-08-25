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
    ~Protocol();

    void setEventCallback(gevent_callback_t, void *userData);
    void setConnectedCallback(gevent_callback_t, void *userData);

    int runEventLoopThreaded();
    void stopEventLoopThread();
    void runEventLoop();
    Uint32 getThreadId();

    bool connectToNode(const char *address);
    void sendProtocolInitiationString();
    int protocolWrite(std::string &str);
    int protocolWrite(const char *str, uint32_t len);
    void sendRequest(std::string);
    void sendRequest(std::string, Value *args);
    void closeConnection();

    bool isFinished();
    void setFinished(bool);
    Value parseJson(std::string &msg);
    std::string stringifyValue(Value *val);
    void handleMessage(std::string &msg);

  private:
    void dispatchEvent(Grids::Event *);

    // callback storage
    gevent_callback_t eventCallback;
    gevent_callback_t connectedCallback;
    void *connectedCallbackUserData;
    void *eventCallbackUserData;

    TCPsocket sock;
    SDL_mutex *finishedMutex;
    SDL_Thread *eventLoopThread;
    bool running;
  };
}

#endif
