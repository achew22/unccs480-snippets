#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <stdio.h>
#include "TestLoader.h"

const int window_width = 640;
const int window_height = 480;


bool key[321];
// Process pending events
bool events() {
}

int main() {


    TestLoader * myTestLoader = new TestLoader();
    return myTestLoader->loop();

}