#ifndef BALL_SERVER_H
#define BALL_SERVER_H

#include <string>

#include "serverConnection.h"

//Forward declaration
#include "app.h"

class BallServer : public ServerConnection{
private:
    App* app;
public:
    BallServer();
    std::string handleConnection(int connectionId);
    void handleData(int connectionId, std::string data);
};

#endif //BALL_SERVER_H
