#include <SDL/SDL.h>

#include "app.h"

#include "GUI.h"
#include "GUIButton.h"
#include "GUITextHandler.h"
#include "camera.h"

#include "carCharacter.h"

/**
 * Default constructor for test loader
 * creates model to draw and then sets up the camera and such
 */
App::App() {
    width = 640;
    height = 480;
    init();

    //Whats a good game without networking?
    //server = new SampleServer(12345);

    //Or characters?
    character = new CarCharacter;

    //Load the object and texture
    character->loadObj("G:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\car.obj");
    character->loadTexture("G:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\textures\\car.bmp");

    //Scale it
    character->scale(1);

    //Define the GUI
    GUI * myGUI = new GUI;

    //Attach a keypress handler
    myGUI->addElement(new GUITextHandler( App::key ));

    //Add it to the GUI
    addGUI("main", myGUI);
    //Activate our GUI
    setActiveGUI("main");

    //Turn on lighting
    enableLighting();

    //Enable light 0 (thats the default one in glLightingStruct())
    enableLight(new glLightingStruct);

    //Set the camera's positions
    camera->setEyePosition(5,5,5);
    camera->setUpDirection(0,0,1);
    //camera->setEyePosition(Point3(.01,.01,5));
    camera->setLookAtPosition(Point3(0,0,0));

    //Tell the camera to follow the character
    //camera->followCharacter(character);
}

/**
 * Display the information to the user
 */
void App::display() {
    //Draw the character
    character->draw();
}

/**
 * Overloaded idle function. Not strictly necessary, but fun to have anyways!
 */
void App::idle() {

}

/**
 * Mess with keys
 */
bool App::key(SDL_Event event) {
    printf("I got a %d that is a/an %c\r\n", event.key.keysym.sym, event.key.keysym.sym);
    switch (event.key.keysym.sym) {
        case SDLK_LEFT:
            App::getInstance()->character->moveLeft(event.type == SDL_KEYDOWN);
            break;
        case SDLK_RIGHT:
            App::getInstance()->character->moveRight(event.type == SDL_KEYDOWN);
            break;
        default:
            break;
    }
    return true;
}

/**
 * Stuff to create instance
 */
App * App::pinstance = 0;
extern App * App::getInstance() {
    if (App::pinstance == 0) {
        App::pinstance = new App();
    }

    return App::pinstance;

}
