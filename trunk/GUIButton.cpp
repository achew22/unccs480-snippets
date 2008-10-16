
#include <string>
#include <vector>

#include <GL/gl.h>

#include "SDLLoader.h"
#include "GUIButton.h"
#include "face.h"

void GUIButton::draw() {

    glPushMatrix();
        glTranslated(x,y,0);
        glColor3d(color.x, color.y, color.z);
        glBegin(GL_QUADS);
            glVertex2f(0,0); //Top left
            glVertex2f(0,height); // Bottom left
            glVertex2f(width,height); // Bottom right
            glVertex2f(width,0); //Top right
        glEnd();
    glPopMatrix();
}

bool GUIButton::key(SDL_Event event) {
    return true;
}

bool GUIButton::mouse(SDL_Event event) {
    int clickX = event.motion.x;
    int clickY = event.motion.y;

    //in the range of the button?
    if ((clickX > x && clickX < (x + width)) && (clickY > y && clickY < (y + height))) {
        if(event.type == SDL_MOUSEBUTTONDOWN && SDL_BUTTON(SDL_GetMouseState(NULL,NULL)) == SDL_BUTTON_LEFT) {
            //Do the callback
            callback();
        }
    }

    return true;
}

GUIButton::GUIButton() {
    x = 0;
    y = 0;
    width = 1;
    height = 1;
}

GUIButton::GUIButton(int setX, int setY, int setWidth, int setHeight, Point3 setTextColor, Point3 setColor, std::string setText, bool (*setCallback)()) {
    x = setX;
    y = setY;
    width = setWidth;
    height = setHeight;
    color = setColor;
    textColor = setTextColor;
    text = setText;
    callback = setCallback;
}
