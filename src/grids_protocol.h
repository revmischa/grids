#include <string>
#include <google/protobuf/message.h>

#include "base.pb.h"
#include "authentication.pb.h"
#include "error.pb.h"
#include "program.pb.h"
#include "grids.pb.h"
#include "node.pb.h"
#include "storage.pb.h"
#include "services.pb.h"
#include "OTR.pb.h"

namespace grids {

  typedef google::protobuf::Message GridsMessage;

class Protocol {
public:
  void set_event_callback(void *, void (*cb)(void *, GridsMessage *));
  void parse_message(const std::string& in_msg);
private:
  void (*event_cb_)(void *, GridsMessage *);
  void *user_data_;
};

}
