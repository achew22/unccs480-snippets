#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <map>

#include "SDLLoader.h"
#include "mesh.h"
#include "textureManager.h"
#include "path.h"
#include "point.h"
#include "sample/ball.h"

class App : public SDLLoader {
private:
    Mesh * myMesh;

    std::map<int, Ball*> balls;

    int texNum;
    Path<Point3>* myPath;

    int upForceId;
    int downForceId;
    int rightForceId;
    int leftForceId;
public:
    static App * pinstance;
    static App * getInstance();

    bool recieveEvent(int id, int direction, bool up);
    static bool key(SDL_Event event);
    static bool click();

    App();

    void display();
    void idle();
    Point3 addBall(int id);
};

#endif // APP_H_INCLUDED

