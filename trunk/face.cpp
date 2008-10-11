#include <GL/gl.h>

#include "face.h"
#include "point.h"

//Empty constructor
Face::Face() {
    vertices = NULL;
    normals = NULL;
    isFan = true;
}

void Face::registerVertices(std::vector<Point3>* toRegister) {
    vertices = toRegister;
}

void Face::registerNormals(std::vector<Point3>* toRegister) {
    normals = toRegister;
}

void Face::registerVertexIndexes(std::vector<int> &toRegister) {
    vertexIndex = toRegister;
}

void Face::registerNormalIndexes(std::vector<int> &toRegister) {
    normalIndex = toRegister;
}

void Face::addVertexIndex(int index) {
    vertexIndex.push_back(index);
}

void Face::addNormalIndex(int index) {
    normalIndex.push_back(index);
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
    if (normals != NULL) {
        for (int i = 0; i < vertexIndex.size(); i++) {
            (*normals)[normalIndex[i]].doNormal();
            (*vertices)[vertexIndex[i]].doVertex();
        }
    } else {
        for (int i = 0; i < vertexIndex.size(); i++) {
            (*vertices)[vertexIndex[i]].doVertex();
        }
    }
    glEnd();
    return true;
}
