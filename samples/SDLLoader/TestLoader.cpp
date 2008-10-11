#include "TestLoader.h"
#include <SDL/SDL.h>

#include "GUI.h"
#include "GUIButton.h"

/**
 * Default constructor for test loader
 * creates model to draw and then sets up the camera and such
 */
TestLoader::TestLoader() {
    width = 640;
    height = 480;
    init();

    //Load up proog so that we can start playing with him
    myMesh = new Mesh();
    myMesh->loadObj("G:\\Documents\\School\\CS480\\unccs480-snippets\\Samples\\ObjLoader\\cube.obj");
    //myMesh->loadObj("AndrewsFilePath");

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
    myGUI->addElement(new GUIButton());

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
    //Set the camera's eye position
    camera->setEyePosition(Point3(.1,.1,10));
}



/**
 * Display the information to the user
 */
void TestLoader::display() {

    //Clear the screen up a little bit before we draw
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw the current mesh
    glPushMatrix(); {
        //Set his color (1,1,0 = Yellow)
        glColor3d(1,1,0);

        //Make him big
        double scaleFactor = 1;
        glScaled(scaleFactor, scaleFactor, scaleFactor);

        //Move to the center
        glTranslated(.8,.7,0);

        //Rotate according to the delta functor defined in init
        glRotated(angle->getValue(), 1,1,0);

        //lets rotate so that it/he/she is looking at us
        glRotated(135, 0,0,1);

        //Draw mesh
        if (!myMesh->drawMesh()) {
            //If things go bad we should cry out in pain
            printf("AHHHHHH PAIN!!!! drawmesh in testloader::display()");
        }
    }
    glPopMatrix();

    //Swap the buffers to show what has happened
    SDL_GL_SwapBuffers();
}

/**
 * Overloaded idle function. Not strictly necessary, but fun to have anyways!
 */
void TestLoader::idle() {

}
