#include <SDL/SDL.h>
#include <cmath>

#include "app.h"

#include "GUI.h"
#include "GUIButton.h"
#include "point.h"
#include "camera.h"

#include "textureManager.h"

#ifndef PI
#define PI 3.1415926
#endif  //PI

/**
 * Default constructor for test loader
 * creates model to draw and then sets up the camera and such
 */
App::App() {
    width = 640;
    height = 640;
    init();

    //Turn on lighting
    enableLighting();

    //Enable light 0 (thats the default one in glLightingStruct())
    enableLight(new glLightingStruct());

    //Create a texture from the file
    myMesh = new Mesh();
    myMesh->loadObj("G:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\car.obj");
    //myMesh->loadObj("D:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\proog.obj");
    myMesh->loadTexture("G:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\textures\\car.bmp");
    myPath = new Path<Point3>(0, 15000);
    for (int i = 0; i < 50; i++) {
        myPath->addPoint(Point3(5*cos(i*2*PI/50.0),5*sin(i*2*PI/50.0),5));
    }
    for (int i = 0; i < 10; i++) {
        myPath->addPoint(Point3(5,0,5*(1 - i/5.0)));
    }
    for (int i = 0; i < 50; i++) {
        myPath->addPoint(Point3(5*cos(i*2*PI/50.0),5*sin(i*2*PI/50.0),-5));
    }
    for (int i = 0; i < 10; i++) {
        myPath->addPoint(Point3(5,0,5*(-1 + i/5.0)));
    }
    myPath->makeCyclic();
    //Dont set the camera at 0, it will disapear
        //This is an error, I will look into it
    //Set the camera's positions
    camera->followPath(*myPath);
    //camera->setEyePosition(5,5,5);
    camera->setUpDirection(0,0,1);
    camera->setLookAtPosition(0,0,0);
}

/**
 * Display the information to the user
 */
void App::display() {
    glClearColor(1,1,1,0);
    //Draw the current mesh
    glPushMatrix(); {
        glColor3f(1,1,1);

        myMesh->drawMesh();

        /*
        //Set the active texture
        glBindTexture(GL_TEXTURE_2D, texNum);

        glBegin(GL_QUADS); {
            glTexCoord2f(1,1);
            //glVertex3f(1,1,0);
            myPath->getPoint().doVertex();
            glTexCoord2f(0,1);
            glVertex3f(-1,1,0);
            glTexCoord2f(0,0);
            glVertex3f(-1,-1,0);
            glTexCoord2f(1,0);
            glVertex3f(1,-1,0);
        }
        glEnd();
        */
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

}

