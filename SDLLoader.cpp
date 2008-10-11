#include <SDL/SDL.h>

#include "SDLLoader.h"

extern SDLLoader * SDLLoader::pinstance = 0;
extern SDLLoader * SDLLoader::getInstance() {
    if (SDLLoader::pinstance == 0) {
        SDLLoader::pinstance = new SDLLoader();
    }
    return SDLLoader::pinstance;

}

/**
 * Default constructor, creates window 640x480 by default
 */
SDLLoader::SDLLoader() {
    width = 640;
    height = 480;
    init();
}

/**
 * Destructor to make sure SDL quits properly
 */
SDLLoader::~SDLLoader() {
    SDL_Quit();
}

/**
 * Non default constructor for SDLLoader, allows height/width definition
 * This probably shouldn't exist since it is always overloaded but exists
 * for the sake of remembing how we were once
 *
 * @param int setWidth width of the window to be created
 * @param int setHeight height of the window to be created
 */
SDLLoader::SDLLoader(int setWidth, int setHeight) {
    width = setWidth;
    height = setHeight;
    init();
}

/**
 * Dispatch key presses to the vector of listening objects
 *
 * @param SDL_Event event the SDL_event that triggered this call
 */
void SDLLoader::dispatchKey( SDL_Event event ) {
    bool used = false;
    for ( uint dispatchIndex = 0; dispatchIndex < dispatchableKeys.size(); ++dispatchIndex ) {
        /**
         * Okay... this is evil.
         *
         * There is a template for a class called DispatchKey (hence why you pass GUI in <>)
         * it accepts in its constructor a pointer to a class of type whatever you passed in
         * as the template. The 2nd param is a reference to the member function that you want
         * to be executed. This is tricky because the result is going to be specificly typed
         * to be "DispatchKey<GUI>" which is not what we want. So, take the resulting object
         * which is a reference to the object. Since this is a pointer it can be stored as an
         * int. Ints are allowed in a vector, so what we are doing is type casting a pointer
         * to an int so that we can hold a lot of unsimilar types in an array then when we
         * need to call them we type cast them back to a type that we know (GUI in this case)
         * so that we can dispatch our event to them, which will most likely be dispatched to
         * another system.
         *
         * Please note that I typecast it to a GUI only becacuse that was the first class that
         * had a class member named keyPress
         */
        if (((DispatchKey<GUI> *)(dispatchableKeys[dispatchIndex]))->keyPress(event) == false) {
            used = true;
            break;
        }
    }
    /**
     * Default behaviors here:
     *
     * Suggested behavior list:
     * Quit
     *   -On q
     *   -On escape
     *
     * Fullscreen
     *   -on f?
     */
    if (used == false) {
        //If there is a q pressed or an escape, quit
        if ((event.key.keysym.sym == SDLK_q) || (event.key.keysym.sym == SDLK_ESCAPE)) {
            quit = 0;
        }
    }
}

/**
 * Very similar to the key press dispatcher, only this
 * dispatches mouse events
 *
 * @param SDL_Event event the SDL_event that triggered this call
 */
void SDLLoader::dispatchMouse(SDL_Event event) {
    for ( uint dispatchIndex = 0; dispatchIndex < dispatchableKeys.size(); ++dispatchIndex ) {
        /**
         * Okay... this is evil.
         *
         * There is a template for a class called DispatchMouse (hence why you pass GUI in <>)
         * it accepts in its constructor a pointer to a class of type whatever you passed in
         * as the template. The 2nd param is a reference to the member function that you want
         * to be executed. This is tricky because the result is going to be specificly typed
         * to be "DispatchMouse<GUI>" which is not what we want. So, take the resulting object
         * which is a reference to the object. Since this is a pointer it can be stored as an
         * int. Ints are allowed in a vector, so what we are doing is type casting a pointer
         * to an int so that we can hold a lot of unsimilar types in an array then when we
         * need to call them we type cast them back to a type that we know (GUI in this case)
         * so that we can dispatch our event to them, which will most likely be dispatched to
         * another system.
         *
         * Please note that I typecast it to a GUI only becacuse that was the first class that
         * had a class member named mouseEvent
         */
        if (((DispatchMouse<GUI> *)(dispatchableMouses[dispatchIndex]))->mouseEvent(event) == false) {
            break;
        }
    }
}

/**
 * Enable lighting in GL, call in conjunction with enableLight (which sets up a light
 */
void SDLLoader::enableLighting() {
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}

/**
 * Enable and configure a light in the current world
 *
 * @param glLightingStruct * myLight class containing all the light variables
 */
void SDLLoader::enableLight(glLightingStruct * myLight) {
    glEnable(myLight->lightNumber);

    glLightfv(myLight->lightNumber, GL_AMBIENT,  myLight->light_ambient);
    glLightfv(myLight->lightNumber, GL_DIFFUSE,  myLight->light_diffuse);
    glLightfv(myLight->lightNumber, GL_SPECULAR, myLight->light_specular);
    glLightfv(myLight->lightNumber, GL_POSITION, myLight->light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   myLight->mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   myLight->mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  myLight->mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, myLight->high_shininess);
}

/**
 * Init GL and SDL so that we can start having fun
 */
bool SDLLoader::init() {
    //Set the quit variable so that we don't exist when loop is called
    quit = -1;

    SDLLoader::pinstance = this;

    // Initialize SDL with best video mode available and degrade to software rendering if we don't have hardware acceleration
    SDL_Init(SDL_INIT_VIDEO);
    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
    int vidFlags = SDL_OPENGL;
    if (videoInfo->hw_available) {
        vidFlags |= SDL_HWSURFACE;
    }
    else {
        vidFlags |= SDL_SWSURFACE;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ) ;

    int bpp = videoInfo->vfmt->BitsPerPixel;
    SDL_SetVideoMode(width, height, bpp, vidFlags);

    camera = new Camera(Point3(5,5,5), Point3(0,0,0), Point3(0,0,1));
    camera->setFrustum(-width / height, width / height, -1, 1, 2, 100);

    return true;
}

/**
 * This is the loop that does the heavy lifting
 * call this to start your SDL application, it will never return control
 * so make sure you don't have any code after it that you want to run
 */
int SDLLoader::loop() {
    while( quit == -1 ) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch( event.type ) {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    //Dispatch the keypress
                    this->dispatchKey(event);
                    break;
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    //Dispatch the mouse event
                    this->dispatchMouse(event);
                    break;
                case SDL_QUIT:
                    quit = 0;
                    break;
                default:
                    break;
            }
        }

        currentTickCount = SDL_GetTicks();
        this->idle();
        camera->update();
        this->display();
    }
    //Program execution is over, time to leave
    SDL_Quit();
    exit(quit);
}

//Virtual overloadable function to allow is to have static components
void SDLLoader::display(){}

//Virtual overloadable function to allow is to have static components
void SDLLoader::idle(){}


/**
 * Return the current time according to the precached time(reset near idle function)
 */
extern int SDLLoader::getTime() {
    return SDLLoader::getInstance()->currentTickCount;
}

/**
 * Add a GUI to the vector of GUIs that is available in the private section of this class
 *
 * @param std::string name of the GUI for later reference
 * @param GUI * GUI to be added to the stack
 */
bool SDLLoader::addGUI(std::string name, GUI * toAdd) {
    guis[name] = toAdd;
    return true;
}

/**
 * Change the active GUI
 *
 * @param std::string name The string representing the GUI to be made active
 */
bool SDLLoader::setActiveGUI(std::string name) {
    activeGUI = name;
    return true;
}

/**
 * Add a key press handler to the SDLLoader dispatcher
 *
 * @param int pointerToKeyPressHandlerToAdd typecasted (to int) pointer to key press handler to add
 */
void SDLLoader::addKeyPressHandler(int pointerToKeyPressHandlerToAdd) {
    dispatchableKeys.push_back(pointerToKeyPressHandlerToAdd);
}

/**
 * Add a mouse handler to the SDLLoader dispatcher
 *
 * @param int pointerToMouseHandlerToAdd typecasted (to int) pointer to mouse handler to add
 */
void SDLLoader::addMouseHandler(int pointerToMouseHandlerToAdd) {
    dispatchableMouses.push_back(pointerToMouseHandlerToAdd);
}
