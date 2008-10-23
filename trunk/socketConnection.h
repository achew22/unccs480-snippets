#ifndef SOCKETCONNECTION_H_INCLUDED
#define SOCKETCONNECTION_H_INCLUDED

#include <string>
#include <vector>
#include "SDL_net.h"

class SocketConnection {
private:

    IPaddress address;
    TCPsocket connection;

    bool connected;

	std::vector< std::string > errors;
	void addError(std::string errorString);
public:

    SocketConnection();

    std::string host;
    int port;

    bool connect();
    bool disconnect();

    std::string lastError();

    bool write(std::string toWrite);
    std::string read(int length);
    std::string read();

};

#endif // SOCKETCONNECTION_H_INCLUDED
