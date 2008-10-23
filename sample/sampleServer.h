#ifndef SAMPLESERVER_H_INCLUDED
#define SAMPLESERVER_H_INCLUDED

#include <string>

#include <SDL/SDL_net.h>

#include "serverConnection.h"

class SampleServer : public ServerConnection {
public:
    SampleServer(int setPort);
    void handleData(int conectionId, std::string data);
};

#endif // SAMPLESERVER_H_INCLUDED
