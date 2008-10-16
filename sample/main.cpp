#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <stdio.h>
#include "app.h"

const int window_width = 640;
const int window_height = 480;


int main(int argc, char *argv[]) {
    App * myApp = new App();
    return myApp->loop();

}
