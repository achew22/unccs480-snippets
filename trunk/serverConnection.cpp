#include "serverConnection.h"

#include <SDL/SDL_net.h>
#include <SDL/SDL_thread.h>

#include "memberThreads.h"
#include "error.h"

/**
 * Set everything up so that we can have a server and start listening
 */
void ServerConnection::init() {

port = 12345;
	// initialize SDL_net
	if(SDLNet_Init()==-1) {
		printf("SDLNet_Init: %s\n",SDLNet_GetError());
		SDL_Quit();
		exit(2);
	}

	// Resolve the argument into an IPaddress type for localhost
	if(SDLNet_ResolveHost(&ip,NULL,port)==-1) {
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		exit(3);
	}

    printf("SUCCK IT TRABECK: %d\n", port);

	// open the server socket
	server=SDLNet_TCP_Open(&ip);
	if(!server) {
		printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		exit(4);
	}

    SDLNet_FreeSocketSet(set);

    //Set up a socket set for the server
    set = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(set,server);

    SDL_CreateMemberThread( this,  &ServerConnection::idle );

}

/**
 * Send data to the client
 *
 * @param clientId The id of the client to send the message to
 * @param message The string to send to the client
 */
//int putMsg(TCPsocket sock, char *buf)
int ServerConnection::send(int clientId, std::string message) {
	Uint32 len,result;

	if(message == "" || message.length() == 0) {
        return(1);
    }

    Error::log("Sending \"%s\" to client %d", message.c_str(), clientId);

    //Add a carrage return linefeed
    message = message + "\r\n";

	// determine the length of the string
	len = message.length() + 1; // add one for the terminating NULL

	// change endianness to network order
	len = SDL_SwapBE32(len);

/*
	// send the length of the string
	result = SDLNet_TCP_Send(clients[clientId], &len, sizeof(len));
	if (result < sizeof(len)) {
		if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		return(0);
	}
*/

	// revert to our local byte order
	//len=SDL_SwapBE32(len);

	// send the buffer, with the NULL as well
	result=SDLNet_TCP_Send(clients[clientId],message.c_str(),message.length());
	if(result<len) {
		if(SDLNet_GetError() && strlen(SDLNet_GetError())) {
		    // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            return(0);
		}
	}

	// return the length sent
	return result;

}

/**
 * This is the thread that deals with the active connections
 * This is probably never going to be called by anyone outside the
 * init function
 *
 * @see init
 */
void ServerConnection::idle() {

	char message[1024];
	int len;

	while (threadQuit == false) {

		//Refresh the set of sockets to allocate a slot for a new client if necessary
		//updateSocketSet();

		//Use CheckSockets to get how many connections have things going on and wait till there is something to do
		//Doing the -1 will make it cast another thread that will wait till there is something to do
		//That way we wont waste mips
		int numready = SDLNet_CheckSockets(set, (Uint32)-1);
		if(numready==-1) {
		    //There was a problem with the socket, its VERY bad and its time to get out
			printf("SDLNet_CheckSockets: %s\n",SDLNet_GetError());
			break;
		}

		if(!numready) {
            //Nothings going on, hang out till something happens
			continue;
		}

		//The server is getting a new connection deal with it
		if(SDLNet_SocketReady(server)) {
			numready--;
			printf("Connection...\n");

			TCPsocket newConnection = SDLNet_TCP_Accept(server);
			if(newConnection) {

				//printf("Accepted...\n");

                //Add the new connection to the socket set
                int clientId = 0; //The default should be 0
                while (clients.count(clientId)) {
                    clientId++;
                }
                printf("Adding a client with the id of %d\n", clientId);
                clients[clientId] = newConnection;

                //Call the callback function to find out what to send to the client
                if (!send(clientId, handleConnection(clientId))) {
                    Error::error("There was an error sending data to client %d on connection", clientId);
                }

                SDLNet_FreeSocketSet(set);
                set = SDLNet_AllocSocketSet(clients.size()+1);

                SDLNet_TCP_AddSocket(set,server);
                for(std::map<int, TCPsocket>::iterator client = clients.begin(); client != clients.end(); client++) {
                    SDLNet_TCP_AddSocket(set,client->second);
                }
			}
		}
        if (clients.size()) {
            //Iterate through all our clients to see whats going on (so long as num ready is still > 0)
            for(std::map<int, TCPsocket>::iterator iter = clients.begin(); numready && (iter != clients.end()); iter++) {

                TCPsocket client = iter->second;

                if(SDLNet_SocketReady(client)) {
                    //Get 1024 bytes and deal with it
                    len = SDLNet_TCP_Recv(client,message,1024);
                    if(!len) {
                        printf("I think this is a closed connection, but if not this is the error: %s\n",SDLNet_GetError());

                        //Remove the person from the client map and close the connection (free some resources)
                        clients.erase(iter);
                        SDLNet_TCP_Close(client);

                        //Reorganize the socket set -- remake it 1 smaller and readd the clients
                        if(set) {
                            SDLNet_FreeSocketSet(set);
                        }
                        set = SDLNet_AllocSocketSet(clients.size()+1);

                        //Iterate through the clients and add them
                        for(std::map<int, TCPsocket>::iterator client = clients.begin(); client != clients.end(); client++) {
                            SDLNet_TCP_AddSocket(set,client->second);
                        }


                    }

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
                    this->handleData(iter->first, toDispatch.substr(0, len));



                }
            }
        }
	}
}

/**
 * Time to quit the server for whatever reason, do it
 */
void ServerConnection::quit() {
    threadQuit = true;
}
