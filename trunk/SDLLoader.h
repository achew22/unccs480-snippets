#ifndef SDLLOADER_H_INCLUDED
#define SDLLOADER_H_INCLUDED

#include <SDL/SDL.h>

#include <vector>
#include <map>

#include "camera.h"
#include "GUI.h"
#include "error.h"

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

/**
 * This is the template that is used to dispatch key events
 * this is bascially a wrapper for object method callbacks
 */
template<class T>
	class DispatchKey {
    private:
        T* _this;
        bool (T::*_keyPress)( SDL_Event a);

    public:
        /**
         * This is the constructor
         * @param the object (by reference [&]) 1st param
         * @param static reference to the method (&myClass::myMethod)
         */
        DispatchKey(T* object, bool (T::*key)( SDL_Event a) ) {
            _this = object;
            _keyPress = key;
        }

        bool keyPress(SDL_Event event) {
            return (_this->*_keyPress)(event);
        }
    };

/**
 * This is the template that is used to dispatch mouse events
 * this is bascially a wrapper for object method callbacks
 */
template<class T>
	class DispatchMouse {
    private:
        T* _this;
        bool (T::*_mouse)( SDL_Event a);

    public:
        /**
         * This is the constructor
         * @param the object (by reference [&]) 1st param
         * @param static reference to the method (&myClass::myMethod)
         */
        DispatchMouse(T* object, bool (T::*mouse)( SDL_Event a) ) {
            _this = object;
            _mouse = mouse;
        }

        bool mouseEvent(SDL_Event event) {
            return (_this->*_mouse)(event);
        }
    };


class SDLLoader {
private:
    void dispatchKey(SDL_Event event);
    void dispatchMouse(SDL_Event event);

    int currentTickCount;


    SDL_Surface * surface;
protected:
    //Make this private soon
    std::vector< int > dispatchableKeys;
    std::vector< int > dispatchableMouses;

    std::map<std::string, GUI *> guis;
    std::string activeGUI;
    int width;
    int height;

    bool init();

    //Add a new keypress handler
    void addKeyPressHandler(int pointerToKeyPressHandlerToAdd);
    void addMouseHandler(int pointerToMouseHandlerToAdd);

    //This is a virtual function with its lack of a defined function explicity stated ( thats the =0 )
    virtual void display();
    virtual void idle();

    int pausedOffset;
    int pausedAt;
    bool paused;

public:
    SDLLoader(int setWidth, int setHeight);
    SDLLoader();
    ~SDLLoader();

    void enableLight(glLightingStruct * myLight);
    void enableLighting();

    bool addGUI(std::string name, GUI * toAdd);
    bool setActiveGUI(std::string name);

    static int getTime();

    //Our camera
    Camera * camera;

    //The quit int
    int quit;

    int loop();


    static SDLLoader * pinstance;
    static SDLLoader * getInstance();

    static bool pause();
    static bool resume();
};

#endif // SDLLOADER_H_INCLUDED
