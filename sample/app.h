#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <SDL/SDL.h>

#include "SDLLoader.h"
#include "mesh.h"

#include "carCharacter.h"
#include "sampleServer.h"

class App : public SDLLoader {
private:
    CarCharacter * character;
    int textureId;

    SampleServer * server;

public:

    static App * pinstance;
    static App * getInstance();

    static bool key(SDL_Event event);

    App();

    void display();
    void idle();
};

#endif // APP_H_INCLUDED
