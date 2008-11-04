#ifndef PLANE_H
#define PLANE_H

#include "physicsObject.h"
#include "point.h"

class Plane : public PhysicsObject {
private:
    Point3 normal;
public:
    Plane() : PhysicsObject() {
        objType = OBJ_TYPE_PLANE;
        isMovable = false;
    }
    Plane(Point3 normalToSet, Point3 centerToSet, double radiusToSet) : PhysicsObject() {
        normal = normalToSet;
        center = centerToSet;
        radius = radiusToSet;

        objType = OBJ_TYPE_PLANE;
        isMovable = false;
    }
};

#endif //PLANE_H
