#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <map>
#include <string>

#include "SDLLoader.h"
#include "point.h"
#include "path.h"
#include "mesh.h"
#include "physicsManager.h"

class PhysicsObject {
protected:
    friend class PhysicsManager;

    Point3 pos;
    Point3 vel;
    Point3 acc;

    Point3 center;
    double radius;

    std::map<int, Path<Point3> > forces;
    int currentIdNumber;

    double mass;
    double maxVel;

    int lastUpdated;
    int increment;

    bool isMovable;
    bool isCollidable;

    int physObjId;
public:
    PhysicsObject() {
        currentIdNumber = 0;
        pos = vel = acc = Point3(0,0,0);
        center = Point3(0,0,0);
        radius = 1;
        mass = 10;
        maxVel = 500;
        lastUpdated = SDLLoader::getTime();
        increment = 10;
        isMovable = isCollidable = true;

        physObjId = PhysicsManager::getInstance()->addObject(this);
    }
    ~PhysicsObject() {
        PhysicsManager::getInstance()->deleteObject(physObjId);
    }
    void setMass(double toSet);
    void setMaxVel(double toSet);
    void setCenter(Point3 toSet);
    void setRadius(double toSet);
    void moveCenterTo(Point3 location);
    void makeMovable();
    void makeImmovable();
    void makeCollidable();
    void makeUncollidable();

    void setVelocity(Point3 toSet);
    void addVelocity(Point3 toAdd);
    int addForce(Path<Point3> toAdd);
    void addGravity(Point3 down);
    bool removeForce(int id);
    void updatePhysics();
};

#endif  //PHYSICSOBJECT_H
