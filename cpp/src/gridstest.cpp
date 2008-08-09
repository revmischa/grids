#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <grids/protocol.h>
#include <grids/event.h>

using namespace Grids;

void gotEvent(Protocol *, Event *, void *);

int main(int argc, char **argv) {
  short finished = 0;
  char buf[256];

  if (argc !=2) {
    std::cerr << "Usage: " << argv[0] << " address\n";
    return -1;
  }

  Protocol *proto = new Protocol();

  gridsmap_t m;
  m["message"] = "LOL HI";

  const char *addr = argv[1];

  if (proto->connectToNode(addr)) {
    std::cout << "Connected!\n";
  } else {
    std::cout << "Could not connect to " << addr << "\n";
  }

  std::string evt = "Debug.Warn";
  proto->sendRequest(evt, &m);

  proto->setEventCallback(gotEvent, NULL);
  proto->runEventLoopThreaded();

  std::cout << "Type quit to exit\n";

  while (! finished) {
    std::cout << "> ";

    fgets(buf, sizeof(buf), stdin);

    if (strncasecmp(buf, "q", 1) == 0 || strncasecmp(buf, "quit", 4) == 0)
      finished = 1;

    if (strncasecmp(buf, "echo", 4) == 0 && strlen(buf) > 5) {
      char *echo = (char *)(buf + 5);
      m.clear();
      m["message"] = echo;
      proto->sendRequest("Debug.Echo", &m);
    }
  }

  proto->stopEventLoopThread();
  proto->closeConnection();
}

void gotEvent(Protocol *proto, Event *evt, void *userData) {
  std::cout << "Received event " << evt->getEventType() << "\n";
}
