#include <iostream>
#include <json/writer.h>
#include "GridsProtocol.h"

namespace Grids {

  std::string Protocol::stringifyMap(gridsmap_t *m) {
    Json::FastWriter *writer = new Json::FastWriter();
    Json::Value root = mapToJsonValue(m);
    return writer->write(root);
  }

  void Protocol::sendRequest(std::string evt) {
    sendRequest(evt, NULL);
  }
  
  void Protocol::sendRequest(std::string evt, gridsmap_t *args) {
    if (args == NULL) {
      args = new gridsmap_t();
    }
  }

  Json::Value Protocol::mapToJsonValue(gridsmap_t *m) {
    std::map<gridskey_t, gridsval_t>::iterator mapIterator;

    Json::Value jsonVal;

    for(mapIterator = m->begin(); 
        mapIterator != m->end();
        mapIterator++) {

      gridskey_t key = mapIterator->first;
      gridsval_t val = mapIterator->second;

      jsonVal[key] = val;
    }

    return jsonVal;
  }

}
