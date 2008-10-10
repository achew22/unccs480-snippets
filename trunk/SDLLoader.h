#ifndef SDLLOADER_H_INCLUDED
#define SDLLOADER_H_INCLUDED

#include <SDL/SDL.h>

#include <vector>
#include <map>

#include "camera.h"
#include "GUI.h"

class glLightingStruct {
public:
    GLfloat light_ambient[4];
    GLfloat light_diffuse[4];
    GLfloat light_specular[4];
    GLfloat light_position[4];

    GLfloat mat_ambient[4];
    GLfloat mat_diffuse[4];
    GLfloat mat_specular[4];
    GLfloat high_shininess[1];

    int lightNumber;

    glLightingStruct() {
            lightNumber = GL_LIGHT0;

            light_ambient[0] = 0.0f;
            light_ambient[1] = 0.0f;
            light_ambient[2] = 0.0f;
            light_ambient[3] = 1.0f;

            light_diffuse[0] = 1.0f;
            light_diffuse[1] = 1.0f;
            light_diffuse[2] = 1.0f;
            light_diffuse[3] = 1.0f;

            light_specular[0] = 1.0f;
            light_specular[1] = 1.0f;
            light_specular[2] = 1.0f;
            light_specular[3] = 1.0f;

            light_position[0] = 2.0f;
            light_position[1] = 5.0f;
            light_position[2] = 5.0f;
            light_position[3] = 0.0f;

            mat_ambient[0] = 0.7f;
            mat_ambient[1] = 0.7f;
            mat_ambient[2] = 0.7f;
            mat_ambient[3] = 1.0f;

            mat_diffuse[0] = 0.8f;
            mat_diffuse[1] = 0.8f;
            mat_diffuse[2] = 0.8f;
            mat_diffuse[3] = 1.0f;

            mat_specular[0] = 1.0f;
            mat_specular[1] = 1.0f;
            mat_specular[2] = 1.0f;
            mat_specular[3] = 1.0f;

            high_shininess[0] = 100.0f;
    }
};

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

    void enableLight(glLightingStruct * myLight);
    void enableLighting();

    bool addGUI(std::string name, GUI * toAdd);
    bool setActiveGUI(std::string name);

    int getTime();

    int loop();
};

#endif // SDLLOADER_H_INCLUDED
