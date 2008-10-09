#ifndef SDLLOADER_H_INCLUDED
#define SDLLOADER_H_INCLUDED

#include <SDL/SDL.h>

#include <vector>
#include <map>

#include "camera.h"
#include "gui.h"


typedef bool(*KeyFunc)(int);
typedef bool(*MouseFunc)(int, int, int);

class SDLLoader {
private:
    void dispatchKey();
    void dispatchMouse();

    std::vector< KeyFunc * > dispatchableKeys;
    //std::vector< bool*() > dispatchableMouses;

    int currentTickCount;

protected:
    Camera * camera;
    std::map<std::string, GUI *> guis;
    std::string activeGUI;
    int width;
    int height;

    //The quit int
    int quit;

    bool init();

    //This is a virtual function with its lack of a defined function explicity stated ( thats the =0 )
    virtual void display() =0;
    virtual void idle() =0;

public:
    SDLLoader(int setWidth, int setHeight);
    SDLLoader();
    ~SDLLoader();

    bool addGUI(std::string name, GUI * toAdd);
    bool setActiveGUI(std::string name);

    int getTime();

    int loop();
};

#endif // SDLLOADER_H_INCLUDED
