#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <grids/GridsProtocol.h>
#include <GridsEvent.h>

void gotGridsEvent(Grids::GEvent *);

int main(int argc, char **argv) {
  short finished = 0;
  char buf[256];

  if (argc !=2) {
    std::cerr << "Usage: " << argv[0] << " address\n";
    return -1;
  }

  Grids::Protocol *proto = new Grids::Protocol();

  Grids::gridsmap_t m;
  m["message"] = "LOL HI";

  const char *addr = argv[1];

  if (proto->connectToNode(addr)) {
    std::cout << "Connected!\n";
  } else {
    std::cout << "Could not connect to " << addr << "\n";
  }

  std::string evt = "Debug.Warn";
  proto->sendRequest(evt, &m);

  proto->runEventLoopThreaded();

  std::cout << "Type quit to exit\n";

  while (! finished) {
    std::cout << "> ";

    fgets(buf, sizeof(buf), stdin);

    if (strncasecmp(buf, "q", 1) == 0 || strncasecmp(buf, "quit", 4) == 0)
      finished = 1;
  }

  proto->stopEventLoopThread();
  proto->closeConnection();
}

void gotGridsEvent(Grids::GEvent *evt) {
  std::cout << "Received event " << evt->getEventType() << "\n";
}
