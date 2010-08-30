#include "grids_protocol.h"
#include <iostream>
#include <string>

namespace grids {
  void Protocol::set_event_callback(void *user_data_in, void (*cb)(void *, GridsMessage *)) {
    user_data_ = user_data_in;
    event_cb_ = cb;
  }
  
  void Protocol::parse_message(const std::string& in_msg) {
    GridsMessage *msg;

    int delim = in_msg.find_first_of('\x01');
    if (delim == -1) {
      // invalid message
      return;
    }
    std::string type = in_msg.substr(0, delim);
    std::string proto = in_msg.substr(delim);
    std::cout << "type: " << type << " proto: " << proto << "\n";

#include "protocol_type_parser.cpp"

    event_cb_(user_data_, msg);
  }
}
