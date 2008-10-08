#include <GL/gl.h>

#include "camera.h"

Camera::Camera()
{
    //Set up defaults
    eyePosition = Point3(0,0,1);
    lookAtPosition = Point3(0,0,-1);
    upDirection = Point3(1,0,0);

    frustum[0] = -1;
    frustum[1] = 1;
    frustum[2] = -1;
    frustum[3] = 1;
    frustum[4] = 2;
    frustum[5] = 100;
}

Camera::Camera(Point3 pos, Point3 lookAt, Point3 up)
{
    eyePosition = pos;
    lookAtPosition = lookAt;
    upDirection = up;

    //Set up default frustum style
    frustum[0] = -1;
    frustum[1] = 1;
    frustum[2] = -1;
    frustum[3] = 1;
    frustum[4] = 2;
    frustum[5] = 100;
}

void Camera::setEyePosition(Point3 pos)
{
    eyePosition = pos;
}

void Camera::setLookAtPosition(Point3 lookAt)
{
    lookAtPosition = lookAt;
}

void Camera::setUpDirection(Point3 up)
{
    upDirection = up;
}

void Camera::setFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    frustum[0] = left;
    frustum[1] = right;
    frustum[2] = bottom;
    frustum[3] = top;
    frustum[4] = zNear;
    frustum[5] = zFar;
}

void Camera::doGlSetDisp()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(frustum[0], frustum[1], frustum[2], frustum[3], frustum[4], frustum[5]);
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z, lookAtPosition.x, lookAtPosition.y, lookAtPosition.z, upDirection.x, upDirection.y, upDirection.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Camera::spinAroundCenter(GLdouble angleUp, GLdouble angleLeft)
{
    //Unfinished
    double radius = (eyePosition - lookAtPosition).getMag();

}

void Camera::spinAroundCamera(GLdouble angleUp, GLdouble angleLeft)
{
}

void Camera::zoomIn(GLdouble amount)
{
    eyePosition = eyePosition * amount;
}

void Camera::shift(Point3 direction, GLdouble amount)
{
}
