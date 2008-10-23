#include "physicsObject.h"
#include "textureManager.h"

void PhysicsObject::setMass(double toSet) {
    mass = toSet;
}

void PhysicsObject::setMaxVel(double toSet) {
    maxVel = toSet;
}

void PhysicsObject::setCenter(Point3 toSet) {
    center = toSet;
}

void PhysicsObject::setRadius(double toSet) {
    radius = toSet;
}

void PhysicsObject::moveCenterTo(Point3 location) {
    Point3 offset = center - pos;
    center = location;
    pos = center - offset;
}


void PhysicsObject::makeMovable() {
    isMovable = true;
}

void PhysicsObject::makeImmovable() {
    isMovable = false;
}

void PhysicsObject::makeCollidable() {
    isCollidable = true;
}

void PhysicsObject::makeUncollidable() {
    isCollidable = false;
}

void PhysicsObject::setVelocity(Point3 toSet) {
    vel = toSet;
    if (vel.getMag() > maxVel) {
        vel = vel.getUnit() * maxVel;
    }
}

void PhysicsObject::addVelocity(Point3 toAdd) {
    vel = vel + toAdd;
    if (vel.getMag() > maxVel) {
        vel = vel.getUnit() * maxVel;
    }
}

int PhysicsObject::addForce(Path<Point3> toAdd) {
    forces.insert(std::pair<int, Path<Point3> >(currentIdNumber, toAdd));
    currentIdNumber++;
    return currentIdNumber - 1;
}

void PhysicsObject::addGravity(Point3 down) {
    Path<Point3> gravity(0,1000);
    gravity.addPoint(down.getUnit()*9.8*mass);
    gravity.makeCyclic();
    forces.insert(std::pair<int, Path<Point3> >(currentIdNumber, gravity));
    currentIdNumber++;
}

bool PhysicsObject::removeForce(int id) {
    std::map<int, Path<Point3> >::iterator iter = forces.find(id);
    if (iter != forces.end()) {
        forces.erase(iter);
        return true;
    } else {
        return false;
    }
}

void PhysicsObject::updatePhysics() {
    //Check for dead forces and clear them out
    for (std::map<int, Path<Point3> >::iterator i = forces.begin(); i != forces.end(); i++) {
        if (i->second.getIsFinished() == true) {
            //printf("PhysicsObject::removed a dead force\n");
            //forces.erase(i);
        }
    }

    int currentTime = SDLLoader::getTime();
    Point3 offset = center - pos;
    for (int i = lastUpdated - lastUpdated%increment; i < currentTime - currentTime%increment; i+= increment) {
        pos = pos + vel * (increment/1000.0);
        if (!PhysicsManager::getInstance()->checkCollisions(physObjId)){
            pos = pos + vel * (increment/1000.0);
        }
        //printf("PhysicsObject::pos is %f, %f, %f\n", pos.x, pos.y, pos.z);
        vel = vel + acc * (increment/1000.0);
        if (vel.getMag() > maxVel) {
            vel = vel.getUnit() * maxVel;
        }
        printf("PhysicsObject::vel is %f, %f, %f\n", vel.x, vel.y, vel.z);
        acc = Point3(0,0,0);
        for (std::map<int, Path<Point3> >::iterator i = forces.begin(); i != forces.end(); i++) {
            acc = acc + i->second.getPoint()/mass;
        }
        //printf("PhysicsObject::acc is %f, %f, %f\n", acc.x, acc.y, acc.z);
    }
    center = pos + offset;
    lastUpdated = currentTime;
    //printf("PhysicsObject::updatePhysics called, forces.size() = %i\n", forces.size());
}
