#include <string>
#include <vector>

#include <SDL/SDL_net.h>

#include "error.h"
#include "socketConnection.h"

SocketConnection::SocketConnection() {
    connected = false;
    host = "";
    port = 80;
}

/**
 * Connect the socket to the given host and port.
 *
 * @return boolean Success
 */
bool SocketConnection::connect() {
    if (connected) {
        return connected;
    }

    if (SDLNet_ResolveHost(&address, host.c_str(), port) == -1) {
        Error::error("SDLNet_Init: %s\n", SDLNet_GetError());
    }

    connection = SDLNet_TCP_Open(&address);
    if(connection) {
        connected = true;
    } else {
        Error::error("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        connected = false;
    }
    return connected;
}

/**
 * Get the last error as a string.
 *
 * @return string Last error
 */
std::string SocketConnection::lastError() {
    if (errors.size()) {
        return errors[errors.size() - 1];
    }
    return "";
}

/**
 * Add error
 *
 * @param errorString Error string
 */
void SocketConnection::addError(std::string errorString) {
    errors.push_back(errorString);
}
/**
 * Write data to the socket.
 *
 * @param toWrite The data to write to the socket
 * @return Success
 */
bool SocketConnection::write(std::string toWrite) {
    if (!connected) {
        if (!connect()) {
            return false;
        }
    }

    int result = SDLNet_TCP_Send(connection, toWrite.c_str(), toWrite.size() + 1);
    if(result < (toWrite.size() + 1)) {
        printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        // It may be good to disconnect sock because it is likely invalid now.
    }

    return true;
}

/**
 * Read data from the socket. Returns false if no data is available or no connection could be
 * established.
 *
 * @param length Optional buffer length to read; defaults to 1024
 */
std::string SocketConnection::read(int length) {
    if (!connected) {
        if (!connect()) {
            return false;
        }
    }

    char stringback[1024];
    int result = SDLNet_TCP_Recv(connection,stringback,length);
    if (result <= 0) {
        // An error may have occured, but sometimes you can just ignore it
        // It may be good to disconnect sock because it is likely invalid now.
    }

    return stringback;
}



/**
 * Disconnect the socket from the current connection.
 *
 * @return boolean Success
 */
bool SocketConnection::disconnect() {
    SDLNet_TCP_Close(connection);
    return true;
}
