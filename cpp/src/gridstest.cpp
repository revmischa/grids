#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <grids/protocol.h>
#include <grids/event.h>

using namespace Grids;

void gotEvent(Protocol *, Event *, void *);
void connected(Protocol *, Event *, void *);

Protocol *proto = new Protocol();

int main(int argc, char **argv) {
  short finished = 0;
  char buf[256];

  if (argc !=2) {
    std::cerr << "Usage: " << argv[0] << " address\n";
    return -1;
  }

  if ( SDL_Init(0) < 0 ) {
    printf("Unable to init SDL: %s\n", SDL_GetError());
    return 1;
  }
  
  if (SDLNet_Init() != 0) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    exit(2);
  }

  proto->setEventCallback(gotEvent, NULL);
  proto->setConnectedCallback(connected, NULL);

  const char *addr = argv[1];

  if (! proto->connectToNode(addr)) {
    std::cout << "Could not connect to " << addr << "\n";
  }

  proto->runEventLoopThreaded();

  std::cout << "Type quit to exit\n";

  Value m;

  while (! finished) {
    std::cout << "> ";

    fgets(buf, sizeof(buf), stdin);

    // remove trailing \n
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = '\0';

    if (strncasecmp(buf, "q", 1) == 0 || strncasecmp(buf, "quit", 4) == 0)
      finished = 1;

    if (strncasecmp(buf, "create", 6) == 0)
      proto->sendRequest("Room.Create", NULL);

    if (strncasecmp(buf, "evt", 3) == 0 && strlen(buf) > 4) {
      char *event = (char *)(buf + 4);
      proto->sendRequest(event, NULL);
    }

    if (strncasecmp(buf, "echo", 4) == 0 && strlen(buf) > 5) {
      char *echo = (char *)(buf + 5);
      m.clear();
      m["message"] = echo;
      proto->sendRequest("Debug.Echo", &m);
    }
  }

  proto->stopEventLoopThread();
  proto->closeConnection();

  SDL_Quit();
  SDLNet_Quit();
}

void gotEvent(Protocol *proto, Event *evt, void *userData) {
  std::cout << "Received event " << evt->getEventType() << "\n";
}

void connected(Protocol *proto, Event *, void *userData) {
  std::cout << "Connected!\n";

  Value m;
  m["message"] = "LOL HI";
  std::string evt = "Debug.Warn";
  proto->sendRequest(evt, &m);
}
