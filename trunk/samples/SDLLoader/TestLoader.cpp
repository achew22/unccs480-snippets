#include "TestLoader.h"
#include <SDL/SDL.h>

TestLoader::TestLoader() {
    width = 640;
    height = 480;
    init();

    myMesh = new Mesh();
    myMesh->loadObj("//home//achew22//Desktop//Now//CS480//unccs480-snippets//samples//ObjLoader//teapot.obj");

    camera->setEyePosition(Point3(50,50,50));
    camera->spinAroundCenter(new Delta_Functor(0, 10000, 0, 3.14 * 2, 1), new Delta_Functor());
}

void TestLoader::display() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3d(.5,.5,.5);
    if (!myMesh->drawMesh()) {
        printf("AHHHHHH PAIN!!!! drawmesh in testloader::display()");
    }

    glFinish();
    SDL_GL_SwapBuffers();
}

void TestLoader::idle() {

}
