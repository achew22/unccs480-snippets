#ifndef BALL_H
#define BALL_H

#include <string>

#include "physicsObject.h"
#include "mesh.h"
#include "point.h"

class Ball : public PhysicsObject {
private:
    friend class PhysicsManager;
    Mesh * mesh;
    Point3 color;
public:
    Ball() : PhysicsObject() {
        center = offset = Point3(0,0,0);
        radius = 0;
        mesh = NULL;
        objType = OBJ_TYPE_BALL;
    }
    Ball(int radiusToSet) : PhysicsObject() {
        radius = radiusToSet;
        mesh = NULL;
        objType = OBJ_TYPE_BALL;
    }
    void calculateCenter();
    void calculateRadius();
    void setColor(Point3 toSet);
    void resize(double newSize);
    void loadObj(std::string filename);
    void loadTexture(std::string filename);
    void draw();
};

#endif //BALL_H
