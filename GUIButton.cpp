
#include <string>

#include <GL/gl.h>

#include "GUIButton.h"
void GUIButton::draw() {

}

bool GUIButton::key(SDL_Event event) {
    printf("I got a %d that is a\an %c\r\n", event.key.keysym.sym, event.key.keysym.sym);
    return true;
}

bool GUIButton::mouse(SDL_Event event) {
    if(event.type == SDL_MOUSEBUTTONDOWN && SDL_BUTTON(SDL_GetMouseState(NULL,NULL)) == SDL_BUTTON_LEFT) {
        printf("You clicked the left mouse button at (%d,%d)\r\n", event.motion.x, event.motion.y);
    }
    return true;
}

GUIButton::GUIButton() {

}
