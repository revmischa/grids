#include <iostream>
#include <json/writer.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "GridsProtocol.h"

namespace Grids {
  Protocol::Protocol() {
    sock = 0;
  }

  bool Protocol::connectToNode(const char *address) {
    // look up host
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1;

    if ((hp = gethostbyname(address)) == NULL) {
      herror("gethostbyname");
      return 0;
    }

    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(GRIDS_PORT);
    addr.sin_family = AF_INET;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
      return 0;
    }

    return 1;
  }

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
