#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <stdio.h>
#include "TestLoader.h"

const int window_width = 640;
const int window_height = 480;


int main(int argc, char *argv[]) {
    TestLoader * myTestLoader = new TestLoader();
    return myTestLoader->loop();

}
