#include "ball.h"

void Ball::calculateCenter() {
    if (mesh != NULL) {
        center = mesh->calculateCenter();
    }
    offset = center - pos;
}

void Ball::calculateRadius() {
    if (mesh != NULL) {
        radius = mesh->calculateAverageRadius();
    }
}

void Ball::resize(double newSize) {
    radius = newSize;
    if (mesh != NULL) {
        mesh->resize(newSize);
    }
}

void Ball::loadObj(std::string filename) {
    if (mesh != NULL) {
        delete mesh;
    }
    mesh = new Mesh;
    mesh->loadObj(filename);
}

void Ball::loadTexture(std::string filename) {
    if (mesh != NULL) {
        mesh->loadTexture(filename);
    }
}

void Ball::draw() {
    //glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
        //glColor3f(1,1,1);

        glTranslated(pos.x, pos.y, pos.z);

        //glScaled(1.0/radius, 1.0/radius, 1.0/radius);

        if (mesh != NULL) {
            mesh->drawMesh();
        }
    glPopMatrix();
    //glPopAttrib();
}
