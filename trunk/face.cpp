#include <GL/glut.h>

#include "face.h"
#include "point.h"

//Empty constructor
Face::Face() {
    isFan = true;
}

void Face::registerPoints(std::vector<Point3>* toRegister) {
    points = toRegister;
}

void Face::registerIndexes(std::vector<int> &toRegister) {
    indexes = toRegister;
}

void Face::addIndex(int index) {
    indexes.push_back(index);
}

void Face::makeFan() {
    isFan = true;
}

void Face::makeStrip() {
    isFan = false;
}

void Face::addNormalToPoints() {
    //Not finished
}

//Draw the face to the screen
bool Face::drawFace() {
    if (isFan) {
        glBegin(GL_TRIANGLE_FAN);
    } else {
        glBegin(GL_TRIANGLE_STRIP);
    }
    for (int i = 0; i < indexes.size(); i++) {
        (*points)[indexes[i]].doNormal();
        (*points)[indexes[i]].doVertex();
    }
    glEnd();
    return true;
}
