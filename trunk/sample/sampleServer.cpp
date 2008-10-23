#include "sampleServer.h"

#include <string>

#include "error.h"
#include "app.h"


SampleServer::SampleServer(int setPort) {
    port = setPort;
    init();
}

void SampleServer::handleData(int conectionId, std::string data) {
    Error::debug("%d: \"%s\"", conectionId, data.c_str());
    if (data == "right") {
        //App::plusX();
    } else if (data == "left") {
        //App::minusX();
    } else if (data == "jump") {
        App * App = App::getInstance();
        //App->y->cat(Delta_Functor(App::getTime(), App::getTime() + 10000, 360, 0, 1);
    }
}
