#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

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

    /**
     * Stolen shamelessly from http://www.gamedev.net/community/forums/topic.asp?topic_id=104791
     * Thank you so much, this is a VERY clever way of doing it
     *
     * This basically sets up a simple 2d orthogo world for us to render into
     * So far as I can tell with the few tweeks I put in there it will adjust everything
     * so that the mouse events that come in from SDL will directly map to the position you are
     * drawing on the screen inside this block of code
     */
    int viewPort[4];
    glGetIntegerv(GL_VIEWPORT, viewPort);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();

        glOrtho(0, viewPort[2], viewPort[3], 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();

            for (unsigned int i = 0; i < elements.size(); i++) {
                elements[i]->draw();
            }

            glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
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
