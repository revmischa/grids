#include <iostream>
#include "grids_protocol.h"

void got_event(void *user_data, grids::GridsMessage *event);

int main() {
  grids::Protocol *p = new grids::Protocol();
  int a = 123;
  p->set_event_callback(&a, got_event);
}

void got_event(void *user_data, grids::GridsMessage *event) {
  std::cout << "Got event, user_data: " << *((int*)user_data) << "\n!";
}


