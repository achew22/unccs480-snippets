#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "../../unccs480-snippets/SDLLoader.h"
#include "../../unccs480-snippets/mesh.h"

class App : public SDLLoader {
private:
    Mesh * myMesh;
    Delta_Functor * angle;

public:

    static bool click();

    App();

    void display();
    void idle();
};

#endif // APP_H_INCLUDED
