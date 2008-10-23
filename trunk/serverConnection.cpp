#include "serverConnection.h"

#include <SDL/SDL_net.h>
#include <SDL/SDL_thread.h>

#include "memberThreads.h"
#include "error.h"

void ServerConnection::init() {
/*
    set = SDLNet_AllocSocketSet(16);
    if (!set) {
        Error::error("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
    }

    // create a listening TCP socket on our port
    if (SDLNet_ResolveHost(&ip,NULL,port) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    server = SDLNet_TCP_Open(&ip);
    if(!server) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }

    //Start listening
    clients[0] = SDLNet_TCP_Accept(server);

    SDLNet_TCP_AddSocket(set, clients[0]);
*/
    SDL_CreateMemberThread( this,  &ServerConnection::idle );

}

void ServerConnection::idle() {

	IPaddress ip,*remoteip;
	TCPsocket server,client;
	char message[1024];
	int len;
	Uint32 ipaddr;
	Uint16 port = 12345;

	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&ip,NULL,port)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		exit(3);
	}

	// open the server socket
	server=SDLNet_TCP_Open(&ip);
	if(!server)
	{
		printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		exit(4);
	}

    int quit = 0;
    while (quit == 0) {


        // try to accept a connection
        client=SDLNet_TCP_Accept(server);
        if(!client) { // no connection accepted
            //printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError());
            SDL_Delay(100); //sleep 1/10th of a second
            continue;
        }

        // get the clients IP and port number
        remoteip=SDLNet_TCP_GetPeerAddress(client);
        if(!remoteip) {
            printf("SDLNet_TCP_GetPeerAddress: %s\n",SDLNet_GetError());
            continue;
        }

        // print out the clients IP and port number
        ipaddr=SDL_SwapBE32(remoteip->host);
        printf("Accepted a connection from %d.%d.%d.%d port %hu\n",
            ipaddr>>24,
            (ipaddr>>16)&0xff,
            (ipaddr>>8)&0xff,
            ipaddr&0xff,
            remoteip->port);

            // read the buffer from client
            len = SDLNet_TCP_Recv(client,message,1024);
            if(!len) {
                printf("SDLNet_TCP_Recv: %s\n",SDLNet_GetError());
                continue;
            }
            SDLNet_TCP_Close(client);

            std::string toDispatch = message;

            /*
                I like to test with netcat and telnet to allow for scripting and granular controll
                This will kill the \r\n that is required to send a message (in linux telnet)
            */
            if (toDispatch[len -1] == 10) { //Catch \r (which is weird
                len--;
            }
            if (toDispatch[len -1] == 13) {
                len--;
            }

            //Cut the extra crap off from the end and dispatch it
            this->handleData(0, toDispatch.substr(0, len));

    }


}
