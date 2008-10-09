#ifndef CAMERA_H
#define CAMERA_H

#ifndef PI  //Defining PI this way allows us to define it wherever we need it and not worry about multiple definitions
#define PI 3.1415926
#endif

#include <Gl/gl.h>

#include "point.h"

//A class to manage where the camera is at all times, makes for nice nice
class Camera
{
private:
    Point3 eyePosition;
    Point3 lookAtPosition;
    Point3 upDirection;

    GLdouble frustum [6];
public:
    Camera();
    Camera(Point3 eye, Point3 lookAt, Point3 up);
    void setEyePosition(Point3 pos);
    void setLookAtPosition(Point3 lookAt);
    void setUpDirection(Point3 up);
    void setFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
    void doGlSetDisp();
    void spinAroundCenter(GLdouble angleUp, GLdouble angleLeft);
    void spinAroundCamera(GLdouble angleUp, GLdouble angleLeft);
    void zoomIn(GLdouble amount);
    void shift(Point3 direction, GLdouble amount);
};

#endif //CAMERA_H