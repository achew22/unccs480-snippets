#include <string>

#include "ballServer.h"
#include "point.h"
#include "app.h"

BallServer::BallServer() {
    port = 12345;
    init();
    app = App::getInstance();
}

std::string BallServer::handleConnection(int connectionId) {
    Point3 returned;// = app->addBall(connectionId);
    if (returned == Point3(-1,-1,-1)) {
        //Kill it
        return "";
    }
    char* toReturn;
    sprintf(toReturn, "v %f %f %f", returned.x, returned.y, returned.z);
    return std::string(toReturn);
}

void BallServer::handleData(int connectionId, std::string data) {

}
