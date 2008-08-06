#include <iostream>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include <json/writer.h>

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

    // hooray we are connnected! initialize protocol
    sendProtocolInitiationString();

    return 1;
  }

  void Protocol::sendProtocolInitiationString() {
    protocolWrite("==Grids/1.0/JSON");
  }

  int Protocol::protocolWrite(const char *str) {
    uint32_t len = strlen(str);
    uint32_t len_net = htonl(len);

    unsigned int outstr_len = len + 5;
    char sep = ',';
    char *outstr = (char *)malloc(outstr_len);

    memcpy(outstr, &len_net, 4);
    memcpy((outstr + 4), &sep, 1);
    memcpy((outstr + 5), str, len);

    return write(sock, outstr, outstr_len);
  }

  void Protocol::closeConnection() {
    shutdown(sock, SHUT_RDWR); 
    close(sock); 
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
    if (evt.empty())
      return;

    if (args == NULL) {
      args = new gridsmap_t();
    }

    (*args)["_method"] = evt.c_str();
    std::string msg = stringifyMap(args);

    protocolWrite(msg.c_str());
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
