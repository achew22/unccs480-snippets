#include <SDL/SDL.h>

#include "app.h"

#include "GUI.h"
#include "GUIButton.h"
#include "point.h"

/**
 * Default constructor for test loader
 * creates model to draw and then sets up the camera and such
 */
App::App() {
    width = 640;
    height = 480;
    init();

    //Load up proog so that we can start playing with him
    myMesh = new Mesh();
    //myMesh->loadObj("/home/achew22/Desktop/Now/CS480/unccs480-snippets/sample/objects/proog.obj");
    //myMesh->loadObj("E:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\proog.obj");
    myMesh->loadObj("./objects/proog.obj");

    //Set up the normals from a calculation, rather than reading them from the file
    myMesh->calculateNormals();

    //Set up the functor that will move proog
    angle = new Delta_Functor(
        0,      //Start time
        10000,  //End time
        0,      //Start value
        360,    //Final value
        1       //What curve to take
    );

    //Make it cyclical so that he will keep spinning
    angle->makeCyclic();

    //Define the gui
    GUI * myGUI = new GUI();
    myGUI->addElement(new GUIButton(  0,  0, 100,100, Point3(0xFF,0xFF,0xFF), Point3(0xFF,0x00,0x00), "Click me?", App::click));
    myGUI->addElement(new GUIButton(  0,100, 100,100, Point3(0xFF,0xFF,0xFF), Point3(0xFF,0x66,0x00), "Click me?", &App::click));
    myGUI->addElement(new GUIButton(  0,200, 100,100, Point3(0xFF,0xFF,0xFF), Point3(0xFF,0xFF,0x00), "Click me?", &App::click));
    myGUI->addElement(new GUIButton(  0,300, 100,100, Point3(0xFF,0xFF,0xFF), Point3(0xCC,0x00,0x66), "Click me?", &App::click));
    myGUI->addElement(new GUIButton(  0,400, 100,100, Point3(0xFF,0xFF,0xFF), Point3(0x00,0xFF,0x00), "Click me?", &App::click));

    addGUI("main", myGUI);
    setActiveGUI("main");

    printf("%d", SDLLoader::getInstance()->getTime());

    /**
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
     */
    /**
     * In this case we are dispatching the generic key catcher for the GUI class, this will
     * sub-dispatch to all GUI elements that it has so that they can do their magic.
     */
    addKeyPressHandler((int)(new DispatchKey<GUI>(myGUI, &GUI::key)));
    addMouseHandler((int)(new DispatchMouse<GUI>(myGUI, &GUI::mouse)));

    //Turn on lighting
    enableLighting();

    //Enable light 0 (thats the default one in glLightingStruct())
    enableLight(new glLightingStruct());


    //Dont set the camera at 0, it will disapear
        //This is an error, I will look into it
    //Set the camera's positions
    camera->setEyePosition(Point3(-5,-5,5));
    camera->setLookAtPosition(Point3(0,0,0));
}



/**
 * Display the information to the user
 */
void App::display() {

    //Draw the current mesh
    glPushMatrix(); {
        //Set his color (1,1,0 = Yellow)
        glColor3d(1,1,0);

        //Make him big
        double scaleFactor = 2;
        glScaled(scaleFactor, scaleFactor, scaleFactor);

        //Move to the center
        glTranslated(0,0,0);

        //Rotate according to the delta functor defined in init

        glRotated(angle->getValue(), 1,1,0);
        glRotated(angle->getValue(), 0,1,1);
        glRotated(angle->getValue(), 1,0,1);

        //lets rotate so that it/he/she is looking at us
        glRotated(135, 0,0,1);


        //Draw mesh
        if (!myMesh->drawMesh()) {
            //If things go bad we should cry out in pain
            printf("AHHHHHH PAIN!!!! drawmesh in App::display()");
        }
    }
    glPopMatrix();
}

/**
 * Overloaded idle function. Not strictly necessary, but fun to have anyways!
 */
void App::idle() {

}

/**
 * Click callback on the buttons
 */
bool App::click() {
    printf("We love clicks\r\n");
}
