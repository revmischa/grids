#pragma once

#include <map>
#include <string>
#include <json/value.h>

namespace Grids {

  const unsigned int GRIDS_PORT = 1488;

  typedef const char * gridskey_t;
  typedef const char * gridsval_t;
  typedef std::map<gridskey_t, gridsval_t> gridsmap_t;

  class Protocol {
  public:
    Protocol();
    bool connectToNode(const char *address);
    std::string stringifyMap(gridsmap_t *m);
    void sendRequest(std::string evt);
    void sendRequest(std::string evt, gridsmap_t *args);

  private:
    Json::Value mapToJsonValue(gridsmap_t *);
    int sock;
    
  };

}

