#ifndef SERVERCONNECTION_H_INCLUDED
#define SERVERCONNECTION_H_INCLUDED

#include <string>
#include <map>

#include <SDL/SDL_net.h>


class ServerConnection {
protected:
    IPaddress ip;
	IPaddress * remoteip;

    TCPsocket server;

    SDLNet_SocketSet set; ///Socket set to hold the connections
    std::map<int, TCPsocket> clients;


    SDL_Thread * idleThread;

    bool threadQuit;

    int port;

public:
    virtual void handleData(int conectionId, std::string data) =0; ///Overloadable function to handle data
    virtual std::string handleConnection(int conectionId) =0; ///Overloadable function to handle data

    int send(int clientId, std::string message); ///Send data to the client

    void init(); ///Init everything
    void idle(); ///Thread for the thread to run in

    void quit(); ///Quit the server and stop listening
};

#endif // SERVERCONNECTION_H_INCLUDED
