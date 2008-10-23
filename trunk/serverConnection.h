#ifndef SERVERCONNECTION_H_INCLUDED
#define SERVERCONNECTION_H_INCLUDED

#include <string>

#include <SDL/SDL_net.h>


class ServerConnection {
protected:
    IPaddress ip;
    TCPsocket server,clients[16];
    SDLNet_SocketSet set;

    SDL_Thread * idleThread;

    int port;

public:
    virtual void handleData(int conectionId, std::string data) =0;

    void init();

    void idle();
};

#endif // SERVERCONNECTION_H_INCLUDED
