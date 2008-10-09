#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>

#include "../../camera.h"
#include "../../point.h"
#include "../../mesh.h"
#include "../../DeltaFunctor.h"

using namespace std;

//globals
double theta = 0.0;
vector<Point3> points;
Delta_Functor* myFunctor;
Mesh* myMesh;
Camera* myCamera;
Point3 clickPosition;

//Constants
const int WIDTH = 800;
const int HEIGHT = 800;
#ifndef PI  //Defining PI this way allows us to define it wherever we need it and not worry about multiple definitions
#define PI 3.1415926
#endif
const double MOUSE_SENSITIVITY = 1.0;

//Lights, don't actually work yet
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

//Function declarations
static void resize(int, int);
void pause();
void display();
void init();
void myIdle();
void mykey(unsigned char key, int x, int y);
void drawCoordAxes(double length);
void createMesh();
void setDrawFigureMode();
void setViewFigureMode();
void drawFigureResize(int width, int height);
void drawFigureKeyHandler(unsigned char key, int x, int y);
void drawFigureMouseHandler(int button, int state, int x, int y);
void drawFigureDisplayHandler();
void drawFigureIdleHandler();

//Stole this from Code::Blocks default
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    //Set up projection mode here. It is apparently safe to use glFrustum and gluLookAt
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    gluLookAt(5,5,5,0,0,0,0,0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

//Do nothing, essentially pause the graphics
void pause()
{
}

//Displays the object - I am moving the camera in the function - Oscillates in a circle around the z-axis
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    myCamera->doGlSetDisp();

    /* Draw Coordinate Axes * /
    glPushMatrix();
        drawCoordAxes(5);
    glPopMatrix();
    /*                      */

    glPushMatrix();
        glRotated(theta, 0, 1, 0);
        glColor3f(1.0, 1.0, 0.0);
        myMesh->drawMesh();
        glColor3f(1.0, 0.0, 0.0);
        //glutSolidSphere(1, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glFrustum(-4.0, 4.0, -4.0, 4.0, 1, 4.0);
    //glTranslated(0,0,4);
    glColor3f(1.0, 0.0, 0.0);

    //Stuff from sample
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_NORMALIZE); //Automatically rescale normals to make them unit length
    glEnable(GL_COLOR_MATERIAL);

    /* Enable lighting */
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    /* */

    // enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    //init functor
    myFunctor = new Delta_Functor(glutGet(GLUT_ELAPSED_TIME) + 1000, glutGet(GLUT_ELAPSED_TIME) + 6000, 0, 360, 1);
    //myFunctor->reverse();
    myFunctor->makeCyclic();

    //Init mesh
    printf("Making a mesh now...\n");
    myMesh = new Mesh();
    myMesh->loadObj("./proog.obj");

    //Init camera
    myCamera = new Camera(Point3(5,5,5), Point3(0,0,0), Point3(0,0,1));
    myCamera->setFrustum(-1, 1, -1, 1, 2, 100);
}

//This function is called when no events are pending.
void myidle() {
    theta = myFunctor->getValue();
    //printf("Time elapsed is %i, theta is %d\n", glutGet(GLUT_ELAPSED_TIME), theta);
    if (theta >= 360.0)
        theta -= 360.0;
    else if (theta <= 0.0)
        theta += 360.0;
    glutPostRedisplay();
}

void mykey(unsigned char key, int x, int y) {
    if(key == 'q')
        exit(0);
    else if (key == 'd')
        setDrawFigureMode();
    else if (key == '+')
        myCamera->zoomIn(0.95);
    else if (key == '-')
        myCamera->zoomIn(1.05);
    else if (key == '4')
        myCamera->spinAroundCenter(0,-PI/10.0);
    else if (key == '6')
        myCamera->spinAroundCenter(0,PI/10.0);
    else if (key == '8')
        myCamera->spinAroundCenter(PI/10.0,0);
    else if (key == '2')
        myCamera->spinAroundCenter(-PI/10.0,0);
}

void mymouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        clickPosition = Point3((x - WIDTH)/(WIDTH/2.0),(y - WIDTH)/(WIDTH/2.0),0);
    }
}

void mymotion(int x, int y)
{
    double shiftx = clickPosition.x - (x - WIDTH)/(WIDTH/2.0);
    double shifty = clickPosition.y - (y - WIDTH)/(WIDTH/2.0);
    clickPosition = Point3((x - WIDTH)/(WIDTH/2.0),(y - WIDTH)/(WIDTH/2.0),0);
    myCamera->spinAroundCenter(-shifty*MOUSE_SENSITIVITY, shiftx*MOUSE_SENSITIVITY);
}

//This will draw the coordinate axes, which is nice
void drawCoordAxes(double length)
{
    glPushMatrix();
        glBegin(GL_LINES);
            glColor3f(0,0,1);
            glLineWidth(2.0);
            glVertex3f(0,0,0);
            glVertex3f(0,0,length);

            glColor3f(0,1,0);
            glVertex3f(0,0,0);
            glVertex3f(0,length,0);

            glColor3f(1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(length,0,0);
        glEnd();
    glPopMatrix();
}

//Create the mesh from the points in points
void createMesh()
{
    if (myMesh != NULL)
    {
        delete myMesh;
    }
    myMesh = new Mesh();
    for (int degree = 0; degree < 360; degree += 10)
    {
        for (int i = 0; i < points.size() - 1; i++)
        {
            vector<Point3> face;
            face.push_back(Point3(points[i + 1].x * cos(degree * 3.14159/180), points[i + 1].x * sin(degree * 3.14159/180), points[i + 1].y));
            face.push_back(Point3(points[i].x * cos(degree * 3.14159/180), points[i].x * sin(degree * 3.14159/180), points[i].y));
            face.push_back(Point3(points[i].x * cos((degree + 10) * 3.14159/180), points[i].x * sin((degree + 10) * 3.14159/180), points[i].y));
            face.push_back(Point3(points[i + 1].x * cos((degree + 10) * 3.14159/180), points[i + 1].x * sin((degree + 10) * 3.14159/180), points[i + 1].y));
            myMesh->addFace(face);
            vector<Point3> reverseFace;
            for (int j = 0; j < face.size(); j++)
            {
                reverseFace.push_back(face[face.size() - 1 - j]);
            }
            myMesh->addFace(reverseFace);
        }
    }
}

//Set the mode to where you are drawing a new figure in 2d
void setDrawFigureMode()
{
    points.resize(0);

    //Set new callback functions
    glutReshapeFunc(drawFigureResize);
    glutKeyboardFunc(drawFigureKeyHandler);
    glutMouseFunc(drawFigureMouseHandler);
    glutMotionFunc(NULL);
    glutDisplayFunc(drawFigureDisplayHandler);
    glutIdleFunc(drawFigureIdleHandler);

    drawFigureResize(800, 800);
}

void setViewFigureMode()
{
    glutReshapeFunc(resize);
    glutKeyboardFunc(mykey);
    glutMouseFunc(mymouse);
    glutMotionFunc(mymotion);
    glutDisplayFunc(display);
    glutIdleFunc(myidle);

    resize(800, 800);
}

void drawFigureResize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    //Set up projection mode here. It is apparently safe to use glFrustum and gluLookAt
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    //gluLookAt(0, -1, 0, 0, 0, 0, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void drawFigureKeyHandler(unsigned char key, int x, int y)
{
    if(key == 'q')
        exit(0);\
    else if (key == 'd')
    {
        createMesh();
        setViewFigureMode();
    }
}

void drawFigureMouseHandler(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        points.push_back(Point3((x - WIDTH/2.0)/(double)(WIDTH/2.0), (-y + HEIGHT/2.0)/(double)(HEIGHT/2.0), 0.0));
        printf("Clicked at %i, %i, corresponding to point %f, %f, %f\n", x, y, (x - WIDTH/2.0)/(double)(WIDTH/2.0), (-y + HEIGHT/2.0)/(double)(HEIGHT/2.0), 0.0);
    }
}

void drawFigureDisplayHandler()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Draw Coordinate Axes * /
    glPushMatrix();
        drawCoordAxes(5);
    glPopMatrix();
    /*                      */

    //Draw a vertical line down the middle
    glPushMatrix();
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
            glVertex3d(0,-1,0);
            glVertex3d(0,1,0);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_STRIP);
            for (int i = 0; i < points.size(); i++)
            {
                points[i].doVertex();
            }
        glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void drawFigureIdleHandler()
{
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    printf("some random numbers: %i, %i, %i, %i, %i\n", rand()%5, rand()%5, rand()%5, rand()%5, rand()%5);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Mesher");

    //Set callback functions
    setViewFigureMode();

    init(); //Initialize everything

    //Enter main loop
    glutMainLoop();

    return 0;
}

