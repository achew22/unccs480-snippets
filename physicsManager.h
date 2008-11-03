#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <map>
#include <stack>

#include "memberThreads.h"

//foward declaration
class PhysicsObject;

class PhysicsManager {
private:
    static PhysicsManager* pinstance;

    std::map<int, PhysicsObject*> objects;
    int currentIdNumber;

    int lastUpdated;

    std::stack< std::pair<int, PhysicsObject*> > objectsToAdd;
    std::stack<int> objectsToDelete;

    //Threads and semaphores
    SDL_Thread* thisThread;
    SDL_sem* objectsToAddSem;
    SDL_sem* objectsToDeleteSem;
public:
    PhysicsManager() {
        currentIdNumber = 0;
        objectsToAddSem = NULL;
        objectsToDeleteSem = NULL;

        thisThread = NULL;
    }
    ~PhysicsManager() {
        if (thisThread != NULL) {
            SDL_KillThread(thisThread);
        }
        if (objectsToAddSem != NULL) {
            SDL_DestroySemaphore(objectsToAddSem);
        }
        if (objectsToDeleteSem != NULL) {
            SDL_DestroySemaphore(objectsToDeleteSem);
        }
    }
    static PhysicsManager* getInstance();
    static void start() {
        PhysicsManager* theInstance = PhysicsManager::getInstance();
        theInstance->objectsToAddSem = SDL_CreateSemaphore(1);
        theInstance->objectsToDeleteSem = SDL_CreateSemaphore(1);

        theInstance->thisThread = SDL_CreateMemberThread( theInstance,  &PhysicsManager::loop );
    }
    void loop();
    int addObject(PhysicsObject* toAdd);
    void deleteObject(int idNum);
    int checkCollisions();
    int checkCollisions(int physObjId);
    int checkCollisions(PhysicsObject* obj1, PhysicsObject* obj2);
};

#endif //PHYSICSMANAGER_H
