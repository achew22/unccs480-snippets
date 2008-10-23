#include "physicsManager.h"
#include "physicsObject.h"

extern PhysicsManager* PhysicsManager::pinstance = NULL;

PhysicsManager* PhysicsManager::getInstance() {
    if (pinstance != NULL) {
        return pinstance;
    } else {
        pinstance = new PhysicsManager;
        return pinstance;
    }
}

int PhysicsManager::addObject(PhysicsObject* toAdd) {
    objects.insert(std::pair<int, PhysicsObject*>(currentIdNumber, toAdd));
    currentIdNumber++;
    return currentIdNumber -1;
}

void PhysicsManager::deleteObject(int idNum) {
    std::map<int, PhysicsObject*>::iterator iter = objects.find(idNum);
    if (iter != objects.end()) {
        objects.erase(iter);
    }
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
            if ((iter1->second->pos - iter2->second->pos).getMag() <= 2) {
                //Then a collision occurred
                printf("Collision occured!\n");
                if (acos(Point3::dot((iter2->second->pos - iter1->second->pos).getUnit(),iter1->second->vel.getUnit())) < 45) {
                    printf("Velocity now is %f, %f, %f\n", iter1->second->vel.x, iter1->second->vel.y, iter1->second->vel.z);
                    iter1->second->vel = iter1->second->vel*-1;
                    printf("Velocity now is %f, %f, %f\n", iter1->second->vel.x, iter1->second->vel.y, iter1->second->vel.z);
                    iter2->second->vel = iter2->second->vel*-1;
                    numCollisions++;
                }
            }
        }
    }
    return numCollisions;
}
