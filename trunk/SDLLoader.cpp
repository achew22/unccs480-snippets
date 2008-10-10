#include <SDL/SDL.h>

#include "SDLLoader.h"


SDLLoader::SDLLoader() {

    width = 640;
    height = 480;
    init();
}

SDLLoader::~SDLLoader() {
    SDL_Quit();
}
SDLLoader::SDLLoader(int setWidth, int setHeight) {
    width = setWidth;
    height = setHeight;
    init();
}

void SDLLoader::dispatchKey() {
    Uint8 * keyMap = SDL_GetKeyState(0);
    for (int i = 0; i < 255; i++) {
        if (keyMap[i]) {
            //std::vector<bool>::iterator itr;
            for ( int dispatchIndex = 0; dispatchIndex < dispatchableKeys.size(); ++dispatchIndex ) {
                if ((*dispatchableKeys[dispatchIndex])(i) == false) {
                    break;
                }
            }
        }
    }
}

void SDLLoader::enableLighting() {
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}

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

void SDLLoader::dispatchMouse() {
    //std::vector< bool > dispatchableMouses(int, int, int);
}

bool SDLLoader::init() {
    //Set the quit flag to false
    quit = -1;

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
}

int SDLLoader::loop() {
    int keysDown = 0;

    while( quit == -1 ) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch( event.type ) {
                case SDL_KEYDOWN:
                    keysDown++;
                    break;
                case SDL_KEYUP:
                    keysDown--;
                    break;
                case SDL_QUIT:
                    quit = 0;
                    break;
                default:
                    break;
            }
            printf("%d\r\n", quit);
        }

        if (keysDown) {
            this->dispatchKey();
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

int SDLLoader::getTime() {
    return currentTickCount;
}

bool addGUI(std::string name, GUI * toAdd) {

}

bool setActiveGUI(std::string name) {

}
