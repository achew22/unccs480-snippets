#ifndef BALL_H
#define BALL_H

#include <string>

#include "physicsObject.h"
#include "mesh.h"

class Ball : public PhysicsObject {
private:
    Mesh * mesh;
public:
    Ball() : PhysicsObject() {
        radius = 0;
        mesh = NULL;
    }
    Ball(int radiusToSet) : PhysicsObject() {
        radius = radiusToSet;
        mesh = NULL;
    }
    void calculateCenter();
    void calculateRadius();
    void loadObj(std::string filename);
    void loadTexture(std::string filename);
    void draw();
};

#endif //BALL_H
