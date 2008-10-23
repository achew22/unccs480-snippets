#ifndef HTTPCONNECTION_H_INCLUDED
#define HTTPCONNECTION_H_INCLUDED

#include <string>

#include "socketConnection.h"

class HTTPConnection : public SocketConnection {
public:
    HTTPConnection();
    std::string get(std::string URL);
};

#endif // HTTPCONNECTION_H_INCLUDED
