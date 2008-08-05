#include <iostream>
#include "GridsProtocol.h"

int main(int argc, char **argv) {
  if (argc !=2) {
    std::cerr << "Usage: " << argv[0] << " address\n";
    return -1;
  }

  Grids::Protocol *proto = new Grids::Protocol();

  Grids::gridsmap_t m;
  m["lol"] = "1";
  m["dongs"] = "2";

  const char *addr = argv[1];

  if (proto->connectToNode(addr)) {
    std::cout << "Connected!\n";
  } else {
    std::cout << "Could not connect to " << addr << "\n";
  }

}
