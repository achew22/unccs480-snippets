#include "physicsManager.h"
#include "physicsObject.h"
#include "SDLLoader.h"
#include "error.h"

extern PhysicsManager* PhysicsManager::pinstance = NULL;

PhysicsManager* PhysicsManager::getInstance() {
    if (pinstance != NULL) {
        return pinstance;
    } else {
        pinstance = new PhysicsManager;
        return pinstance;
    }
}

void PhysicsManager::loop() {
    while (1) {
        //Add any objects that need to be added
        SDL_SemWait(objectsToAddSem); //Only access this data if there isn't a lock on it, otherwise just skip it
        while (!objectsToAdd.empty()) {
            objects.insert(objectsToAdd.top());
            objectsToAdd.pop();
        }
        SDL_SemPost(objectsToAddSem);

        //Remove any objects that need to be removed
        SDL_SemWait(objectsToDeleteSem); //Only access this data if there isn't a lock on it, otherwise just skip it
        while (!objectsToDelete.empty()) {
            std::map<int, PhysicsObject*>::iterator iter = objects.find(objectsToDelete.top());
            objectsToAdd.pop();
            if (iter != objects.end()) {
                objects.erase(iter);
            }
        }
        SDL_SemPost(objectsToDeleteSem);

        //Now just update everything individually
        int currentTime = SDLLoader::getTime();
        //First update the positions
        for (int i = lastUpdated - lastUpdated%10; i < currentTime - currentTime%10; i+= 10) {
            for (std::map<int, PhysicsObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++) {
                iter->second->updateTick(10);
                //Check collisions
                for (std::map<int, PhysicsObject*>::iterator iter2 = objects.begin(); iter2 != objects.end(); iter2++) {
                    if (iter != iter2) {
                        if (checkCollisions(iter->second, iter2->second) != 0) {
                            iter->second->resetPosition(currentTime);
                            iter2->second->resetPosition(currentTime);
                        }
                    }
                }
            }
            currentTime = SDLLoader::getTime();
        }
        //Now update the velocities and accelerations of everything
        for (std::map<int, PhysicsObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++) {
            iter->second->updateVel(currentTime);
            iter->second->updateAcc(currentTime);
            //Check for dead forces and clear them out
            for (std::map<int, Path<Point3> >::iterator i = iter->second->forces.begin(); i != iter->second->forces.end(); i++) {
                if (i->second.getIsFinished() == true) {
                    //printf("PhysicsObject::removed a dead force\n");
                    //forces.erase(i);
                }
            }
            iter->second->lastUpdated = currentTime;
        }
        lastUpdated = currentTime;
    }
}

int PhysicsManager::addObject(PhysicsObject* toAdd) {
    SDL_SemWait(objectsToAddSem);
    PhysicsManager::getInstance()->objectsToAdd.push(std::pair<int, PhysicsObject*>(currentIdNumber, toAdd));
    currentIdNumber++;
    SDL_SemPost(objectsToAddSem);
    return currentIdNumber -1;
}

void PhysicsManager::deleteObject(int idNum) {
    SDL_SemWait(objectsToDeleteSem);
    objectsToDelete.push(idNum);
    SDL_SemPost(objectsToDeleteSem);
}

int PhysicsManager::checkCollisions() {
    int numCollisions = 0;
    //for (std::map<int, PhysicsObject*>::iterator iter1 = objects.begin(); iter1 != objects.end(); iter1++) {
    std::map<int, PhysicsObject*>::iterator iter1 = objects.begin();
        for (std::map<int, PhysicsObject*>::iterator iter2 = objects.begin(); iter2 != objects.end(); iter2++) {
            if (iter1 != iter2) {
                if ((iter1->second->pos - iter2->second->pos).getMag() <= 2) {
                    //Then a collision occurred
                    printf("Collision occured!\n");
                    printf("Velocity now is %f, %f, %f\n", iter1->second->vel.x, iter1->second->vel.y, iter1->second->vel.z);
                    iter1->second->vel = iter1->second->vel*-1;
                    printf("Velocity now is %f, %f, %f\n", iter1->second->vel.x, iter1->second->vel.y, iter1->second->vel.z);
                    iter2->second->vel = iter2->second->vel*-1;
                }
            }
        }
    //}
}

int PhysicsManager::checkCollisions(int physObjId) {
    int numCollisions = 0;
    std::map<int, PhysicsObject*>::iterator iter1 = objects.find(physObjId);
    for (std::map<int, PhysicsObject*>::iterator iter2 = objects.begin(); iter2 != objects.end(); iter2++) {
        if (iter1 != iter2) {
            if ((iter1->second->center - iter2->second->center).getMagSquared() <= 4) {
                //Then a collision occurred
                PhysicsObject* ball1 = iter1->second;
                PhysicsObject* ball2 = iter2->second;

                //There are really two velocities per ball - the part pointing directly at the other ball, and the part pointing perpendicular to the first part. These shall be called effecVel and perpVel, respectively.
                Point3 displacement = (ball1->center - ball2->center).getUnit();
                Point3 effecVel1 = displacement * Point3::dot(ball1->vel,displacement);
                Point3 effecVel2 = displacement * Point3::dot(ball2->vel,displacement);
                Point3 perpVel1 = ball1->vel - effecVel1;
                Point3 perpVel2 = ball2->vel - effecVel2;

                /*
                Point3 momentum = iter1->second->vel * iter1->second->mass + iter2->second->vel * iter2->second->mass;
                Point3 direction2 = (iter2->second->pos - iter1->second->pos).getUnit();
                iter2->second->vel = (direction2 * momentum.getMag()) / iter2->second->mass;
                iter1->second->vel = (momentum - iter2->second->vel * iter2->second->mass) / iter1->second->mass;
                */
                ball1->vel = (effecVel1 * ((ball1->mass - ball2->mass)/(ball1->mass + ball2->mass))) + (effecVel2 * ((2 * ball2->mass)/(ball1->mass + ball2->mass))) * 1.0 + perpVel1;
                ball2->vel = (effecVel2 * ((ball2->mass - ball1->mass)/(ball1->mass + ball2->mass))) + (effecVel1 * ((2 * ball1->mass)/(ball1->mass + ball2->mass))) * 1.0 + perpVel2;

                numCollisions++;
            }
        }
    }
    return numCollisions;
}

int PhysicsManager::checkCollisions(PhysicsObject* obj1, PhysicsObject* obj2) {
    int numCollisions = 0;
    if ((obj1->center - obj2->center).getMagSquared() <= obj1->radius*obj1->radius + obj2->radius*obj2->radius) {
        //Then a collision occurred
        //Error::debug("PhysicsManager::Collision occurred!\n    Positions are <%f, %f, %f> and <%f, %f, %f>");
        //There are really two velocities per obj - the part pointing directly at the other obj, and the part pointing perpendicular to the first part. These shall be called effecVel and perpVel, respectively.
        Point3 displacement = (obj1->center - obj2->center).getUnit();
        Point3 effecVel1 = displacement * Point3::dot(obj1->vel,displacement);
        Point3 effecVel2 = displacement * Point3::dot(obj2->vel,displacement);
        Error::debug("effecVel = <%f, %f, %f>\n", effecVel1.x, effecVel1.y, effecVel1.z);
        Point3 perpVel1 = obj1->vel - effecVel1;
        Point3 perpVel2 = obj2->vel - effecVel2;

        /*
        Point3 momentum = obj1->vel * obj1->mass + obj2->vel * obj2->mass;
        Point3 direction2 = (obj2->pos - obj1->pos).getUnit();
        obj2->vel = (direction2 * momentum.getMag()) / obj2->mass;
        obj1->vel = (momentum - obj2->vel * obj2->mass) / obj1->mass;
        */
        obj1->vel = (effecVel1 * ((obj1->mass - obj2->mass)/(obj1->mass + obj2->mass))) + (effecVel2 * ((2 * obj2->mass)/(obj1->mass + obj2->mass))) * 0.8 + perpVel1;
        obj2->vel = (effecVel2 * ((obj2->mass - obj1->mass)/(obj1->mass + obj2->mass))) + (effecVel1 * ((2 * obj1->mass)/(obj1->mass + obj2->mass))) * 0.8 + perpVel2;

        numCollisions++;
    }
    return numCollisions;
}
