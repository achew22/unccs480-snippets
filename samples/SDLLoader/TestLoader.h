#ifndef TESTLOADER_H_INCLUDED
#define TESTLOADER_H_INCLUDED

#include "../../unccs480-snippets/SDLLoader.h"
#include "../../unccs480-snippets/mesh.h"

class TestLoader : public SDLLoader {
private:
    Mesh * myMesh;
    Delta_Functor * angle;

public:
    TestLoader();

    void display();
    void idle();
};

#endif // TESTLOADER_H_INCLUDED
