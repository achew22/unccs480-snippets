#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <map>

//foward declaration
class PhysicsObject;

class PhysicsManager {
private:
    static PhysicsManager* pinstance;

    std::map<int, PhysicsObject*> objects;
    int currentIdNumber;
public:
    PhysicsManager() {
        currentIdNumber = 0;
    }
    static PhysicsManager* getInstance();
    int addObject(PhysicsObject* toAdd);
    void deleteObject(int idNum);
    int checkCollisions();
    int checkCollisions(int physObjId);
};

#endif //PHYSICSMANAGER_H
