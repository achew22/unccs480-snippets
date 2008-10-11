#include <SDL/SDL.h>

#include "GUI.h"
#include "SDLLoader.h"

/**
 * Default constructor, set the defaults for everything
 * NOTE: The default size is 640x480
 */
GUI::GUI() {
    xOffset = 0;
    yOffset = 0;
    width = 640;
    height = 480;
    xMax = 1;
    yMax =1;
}

/**
 * Go through all the GUI elements defined and tell them to draw themselvs
 * Do this after setting up the proper 2D orthoganal
 */
bool GUI::draw() {
    for (unsigned int i = 0; i < elements.size(); i++) {
        elements[i]->draw();
    }
}

/**
 * Add an element to the stack of elements to draw
 */
int GUI::addElement(GUIElement * toAdd){
    elements.push_back(toAdd);
}

/**
 * Handle key press events
 * Only called by SDLLoader, expects a SDL_Event of type keypress
 */
bool GUI::key(SDL_Event event) {
    for ( unsigned int dispatchIndex = 0; dispatchIndex < elements.size(); ++dispatchIndex ) {
        if (elements[dispatchIndex]->key(event) == false) {
            return false;
        }
    }
    return true;
}

/**
 * Handle mouse events
 * Only called by SDLLoader, expects a SDL_Event of type mouse event
 */
bool GUI::mouse(SDL_Event event) {
    for ( unsigned int dispatchIndex = 0; dispatchIndex < elements.size(); ++dispatchIndex ) {
        if (elements[dispatchIndex]->mouse(event) == false) {
            return false;
        }
    }
    return true;
}
