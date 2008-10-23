#include <SDL/SDL.h>

#include "SDLLoader.h"
#include "textureManager.h"

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
    TextureManager::cleanUp();
}

/**
 * Non default constructor for SDLLoader, allows height/width definition
 * This probably shouldn't exist since it is always overloaded but exists
 * for the sake of remembing how we were once
 *
 * @param setWidth width of the window to be created
 * @param setHeight height of the window to be created
 */
SDLLoader::SDLLoader(int setWidth, int setHeight) {
    width = setWidth;
    height = setHeight;
    init();
}

/**
 * Dispatch key presses to the vector of listening objects
 *
 * @param event the SDL_event that triggered this call
 */
void SDLLoader::dispatchKey( SDL_Event event ) {
    /**
     * Step 1: dispatch to the GUI, it has first dibs
     */
    if (activeGUI != "none") {
        if (!this->guis[activeGUI]->key(event)) {
            //The GUI took it and told us not to go on
            return;
        }
    }
/*
    bool used = false;

    for ( unsigned int dispatchIndex = 0; dispatchIndex < dispatchableKeys.size(); ++dispatchIndex ) {
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
         * /
        if (((DispatchKey<GUI> *)(dispatchableKeys[dispatchIndex]))->keyPress(event) == false) {
            used = true;
            break;
        }
    }*/
    /**
     * Do the default behaviors (Q is quit and so is ESC)
     */

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
    //If there is a q pressed or an escape, quit
    if ((event.key.keysym.sym == SDLK_q) || (event.key.keysym.sym == SDLK_ESCAPE)) {
        quit = 0;
    }
    if (event.key.keysym.sym == SDLK_f) {
        // Since we saved the surface its not hard to go to fullscreen!
        // This doesn't work look into it
        //SDL_WM_ToggleFullScreen( surface );
    }
}

/**
 * Very similar to the key press dispatcher, only this
 * dispatches mouse events
 *
 * @param event the SDL_event that triggered this call
 */
void SDLLoader::dispatchMouse(SDL_Event event) {
    for ( unsigned int dispatchIndex = 0; dispatchIndex < dispatchableKeys.size(); ++dispatchIndex ) {
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
 * @param myLight class containing all the light variables
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


    /**
     * In windows SDL dumps to a file instead of the console, this is a fix
     * to allow you to see the console values. This should work to make it
     * show up in the console
     */
     /*
#ifdef _WIN32
    const char * confile = "CONOUT$";

    FreeConsole();
    if(  !AllocConsole() ){
        // handle error
    }

    freopen(confile, "w", stdout);
    freopen(confile, "w", stderr);

#endif
    */


    //Init paused variables
    paused = false;
    pausedAt = 0;
    pausedOffset = 0;

    //Set it so that it wont try to do a gui if there is none active
    activeGUI = "none";

    /**
     * This allows for static reachins into the SDLLoader variables like the current time
     * refereneced in getInstance.
     * Example: (getting the time since SDL was started)
     * printf("%d", SDLLoader::getInstance()->getTime());
     *
     * Bad example because we overloaded SDLLoader::getTime() but same idea
     */
    SDLLoader::pinstance = this;

    // Initialize SDL with best video mode available and degrade to software rendering if we don't have hardware acceleration
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("There was an error initilizing SDL\r\n");
        exit(1);
    }

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
    surface = SDL_SetVideoMode(width, height, bpp, vidFlags);

    //Set up the default camera behavior
    camera = new Camera(Point3(5,5,5), Point3(0,0,0), Point3(0,0,1));
    camera->setFrustum(-width / height, width / height, -1, 1, 2, 100);

    //Enable various GL things here
    //Enable culling, so that the reverse of faces are not drawn
    //If we don't enable culling, weird things happen
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //Faces are to be drawn in the counterclockwise direction, since .obj files are oriented in such a direction
    glFrontFace(GL_CCW);

    //I'm not sure what exactly this does, but it's important
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Enable textures
    glEnable(GL_TEXTURE_2D);

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

        //Clear the screen up a little bit before we draw
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->display();
        if (this->activeGUI != "none") {
            //This makes no sense, somebody help me out here
            //So I can check the thing: even when activeGui == "none", it will
            //go into this loop --- unless I am running the debugger, and then
            //it won't. So I put this idiotic second check in to make sure that
            //everyone is happy and stupid.
            if (guis.find(this->activeGUI) != guis.end()) {
                guis[this->activeGUI]->draw();
            }
        }
        //Swap the buffers to show what has happened
        SDL_GL_SwapBuffers();
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
 * Add a GUI to the vector of GUIs that is available in the private section of this class
 *
 * @param name of the GUI for later reference
 * @param toAdd to be added to the stack
 */
bool SDLLoader::addGUI(std::string name, GUI * toAdd) {
    guis[name] = toAdd;
    return true;
}

/**
 * Change the active GUI
 *
 * @param name The string representing the GUI to be made active
 */
bool SDLLoader::setActiveGUI(std::string name) {
    activeGUI = name;
    return true;
}

/**
 * Add a key press handler to the SDLLoader dispatcher
 *
 * @param pointerToKeyPressHandlerToAdd typecasted (to int) pointer to key press handler to add
 */
void SDLLoader::addKeyPressHandler(int pointerToKeyPressHandlerToAdd) {
    dispatchableKeys.push_back(pointerToKeyPressHandlerToAdd);
}

/**
 * Add a mouse handler to the SDLLoader dispatcher
 *
 * @param pointerToMouseHandlerToAdd typecasted (to int) pointer to mouse handler to add
 */
void SDLLoader::addMouseHandler(int pointerToMouseHandlerToAdd) {
    dispatchableMouses.push_back(pointerToMouseHandlerToAdd);
}


/**
 * Return the current time according to the precached time(reset near idle function)
 */
extern int SDLLoader::getTime() {
    SDLLoader * instance = SDLLoader::getInstance();
    if (instance->paused) {
        return instance->pausedAt;
    }
    return instance->currentTickCount - instance->pausedOffset;
}

/**
 * Pause the game
 */
bool SDLLoader::pause() {
    SDLLoader * instance = SDLLoader::getInstance();
    if (!instance->paused) {
        instance->pausedAt = SDLLoader::getTime();
        instance->paused = true;
    }
}

/**
 * Resume the game
 */
bool SDLLoader::resume() {
    SDLLoader * instance = SDLLoader::getInstance();
    if (instance->paused) {
        instance->pausedOffset = SDL_GetTicks() - instance->pausedAt;
        instance->paused = false;
    }
}
