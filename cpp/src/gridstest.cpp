#include <iostream>
#include <grids/GridsProtocol.h>

int main(int argc, char **argv) {
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

  proto->closeConnection();
}
