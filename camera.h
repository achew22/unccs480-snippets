#ifndef CAMERA_H
#define CAMERA_H

#ifndef PI  //Defining PI this way allows us to define it wherever we need it and not worry about multiple definitions
#define PI 3.1415926
#endif

#include <GL/gl.h>

#include "point.h"
#include "DeltaFunctor.h"

//A class to manage where the camera is at all times, makes for nice nice
class Camera
{
private:
    Point3 eyePosition; ///<the position of the camera
    Point3 lookAtPosition; ///<the location that the camera is looking at
    Point3 upDirection; ///<the direction which is 'up', with reference to the camera

    Delta_Functor * angleUpFunct; ///<deprecated, do not use
    Delta_Functor * angleLeftFunct; ///<deprecated, do not use

    GLdouble frustum [6];
public:
    Camera();
    Camera(Point3 eye, Point3 lookAt, Point3 up);
    void setEyePosition(Point3 pos);
    void setLookAtPosition(Point3 lookAt);
    void setUpDirection(Point3 up);
    void setFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
    void update();

    //New motion functions move on surface of sphere around center
    void spinRightAroundCenter(GLdouble angle);
    void spinUpAroundCenter(GLdouble angle);
    void spinViewAroundCenter(GLdouble angle);

    void zoomIn(GLdouble amount);
    void shift(Point3 direction, GLdouble amount);
};

#endif //CAMERA_H
