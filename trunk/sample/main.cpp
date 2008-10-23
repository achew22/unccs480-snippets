#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "app.h"

int main() {
    /*App * myApp = new App();
    return myApp->loop();
    */
    return App::getInstance()->loop();
}
