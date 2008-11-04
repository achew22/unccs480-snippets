#include <SDL/SDL.h>
#include <cmath>

#include "app.h"

#include "GUI.h"
#include "GUIButton.h"
#include "GUITextHandler.h"
#include "point.h"
#include "camera.h"
#include "physicsManager.h"

#include "textureManager.h"
#include "ballServer.h"

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

    BallServer* myBallServer = new BallServer;

    //Turn on lighting
    enableLighting();

    //Enable light 0 (thats the default one in glLightingStruct())
    enableLight(new glLightingStruct());

    //Define a gui
    GUI* myGui = new GUI;
    myGui->addElement(new GUITextHandler( App::key ));
    addGUI("main", myGui);
    setActiveGUI("main");

    //Create a texture from the file
    /*
    myMesh = new Mesh();
    myMesh->loadObj("E:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\car.obj");
    //myMesh->loadObj("D:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\proog.obj");
    myMesh->loadTexture("E:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\textures\\car.bmp");
    */

    //Path for camera
    myPath = new Path<Point3>(0, 15000);
    for (int i = 0; i < 50; i++) {
        myPath->addPoint(Point3(i*cos(i*2*PI/50.0),i*sin(i*2*PI/50.0),i));
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

    //New physics object
    Point3 color = addBall(0);
    printf("Color is %f, %f, %f\n", color.x, color.y, color.z);

    /*
    Ball* ball1 = new Ball;
    ball1->loadObj("G:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\earth.obj");
    //ball1->loadTexture("G:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\textures\\car.bmp");
    ball1->calculateCenter();
    ball1->calculateRadius();
    ball1->resize(1);
    ball1->setMass(1);
    ball1->moveCenterTo(Point3(10,0,11));
    Path<Point3> force1(0,2000);
    force1.addPoint(Point3(-5,0,-5));
    //force1.makeCyclic();
    ball1->addForce(force1);
    //Path<Point3> force2(2000,4000);
    //force2.addPoint(Point3(0,0,50));
    //ball1->addForce(force2);
    //ball1->addGravity(Point3(0,0,-1));
    //ball1->setVelocity(Point3(0,-1,0));
    myBalls.push_back(ball1);
    */

    //Dont set the camera at 0, it will disapear
        //This is an error, I will look into it
    //Set the camera's positions
    //camera->followPath(*myPath);
    camera->setEyePosition(0,0,50);
    camera->setUpDirection(0,1,0);
    camera->setLookAtPosition(0,0,0);

    PhysicsManager::start();
    PhysicsManager::getInstance()->setDamping(0.995);
}

/**
 * Display the information to the user
 */
void App::display() {
    glClearColor(1,1,1,0);

    for (std::map<int, Ball*>::iterator iter = balls.begin(); iter != balls.end(); iter++) {
        iter->second->draw();
    }

    //Draw the current mesh
    glPushMatrix(); {
        glColor3f(1,1,1);

        //myMesh->drawMesh();

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

Point3 App::addBall(int id) {
    if (balls.find(id) != balls.end()) {
        return Point3(-1,-1,-1);
    }
    //Find a place to put the ball
    Ball* newBall = new Ball();
    newBall->loadObj("G:\\Documents\\School\\CS480\\unccs480-snippets\\sample\\objects\\earth.obj");
    newBall->calculateCenter();
    newBall->calculateRadius();
    srand(id);
    Point3 color(1.0/(rand()%5 + 1),1.0/(rand()%5 + 1),1.0/(rand()%5 + 1));
    newBall->setColor(color);
    newBall->resize(1);
    newBall->setMass(1);
    newBall->moveCenterTo(Point3(0,0,10));
    balls[id] = newBall;
    return color;
}

bool App::recieveEvent(int id, int direction, bool up) {
    if (balls.find(id) == balls.end()) {
        return false;
    }
    Path<Point3> newForce(0,1000);
    if (up) {
        switch (direction) {
            case SDLK_LEFT:
                newForce.addPoint(Point3(-50,0,0));
                newForce.makeCyclic();
                this->leftForceId = this->balls[id]->addForce(newForce);
                break;
            case SDLK_RIGHT:
                newForce.addPoint(Point3(50,0,0));
                newForce.makeCyclic();
                this->rightForceId = this->balls[id]->addForce(newForce);
                break;
            case SDLK_DOWN:
                newForce.addPoint(Point3(0,-50,0));
                newForce.makeCyclic();
                this->downForceId = this->balls[id]->addForce(newForce);
                break;
            case SDLK_UP:
                newForce.addPoint(Point3(0,50,0));
                newForce.makeCyclic();
                this->upForceId = this->balls[id]->addForce(newForce);
                break;
            default:
                break;
        }
    } else {
        switch (direction) {
            case SDLK_LEFT:
                this->balls[id]->removeForce(this->leftForceId);
                break;
            case SDLK_RIGHT:
                this->balls[id]->removeForce(this->rightForceId);
                break;
            case SDLK_DOWN:
                this->balls[id]->removeForce(this->downForceId);
                break;
            case SDLK_UP:
                this->balls[id]->removeForce(this->upForceId);
                break;
            default:
                break;
        }
    }
}

/**
 *
 */
bool App::key(SDL_Event event) {
    App::getInstance()->recieveEvent(0, event.key.keysym.sym, event.type == SDL_KEYDOWN);
    return true;
}

/**
 * Click callback on the buttons
 */
bool App::click() {

}

App * App::pinstance = NULL;
extern App * App::getInstance() {
    if (App::pinstance == 0) {
        App::pinstance = new App();
    }

    return App::pinstance;

}
