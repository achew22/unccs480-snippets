#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>

#include "camera.h"
#include "error.h"

#ifndef PI
#define PI 3.1415926
#endif //PI

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

    angleLeftFunct = angleUpFunct = NULL;
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

    angleLeftFunct = angleUpFunct = NULL;
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

void Camera::update()
{
    if ((angleLeftFunct != NULL) && (angleUpFunct != NULL)) {
        spinAroundCenter(angleUpFunct->getValue(), angleLeftFunct->getValue());
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(frustum[0], frustum[1], frustum[2], frustum[3], frustum[4], frustum[5]);
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z, lookAtPosition.x, lookAtPosition.y, lookAtPosition.z, upDirection.x, upDirection.y, upDirection.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*******************Movement***********************
 * These next three functions handle a nice form of movement in which
 * the camera moves as though on the surface of a sphere centered at
 * the lookAtPosition.
 **************************************************/

/**
 * Function: spinRightAroundCenter
 *
 * SpinRightAroundCenter moves (as you would
 * expect) to the right, from the perspective of the viewer.
 *
 * Params:
 *   angle - the angle in degrees that you wish to rotate by
 */
void Camera::spinRightAroundCenter(GLdouble angle) {
    double radius = (eyePosition - lookAtPosition).getMag();

    //Set up some basis vectors for the plane in which you want to move
    Point3 basisY = (eyePosition - lookAtPosition).getUnit();
    Point3 basisX = Point3::cross(basisY, upDirection).getUnit();

    //Rotate the basis vectors by angle, this is the vector to translate
    //to our final destination
    Point3 returnVector = basisX*cos((angle+90)*(PI/180.0)) + basisY*sin((angle+90)*(PI/180.0));

    //Translate to the final position
    eyePosition = lookAtPosition + returnVector.getUnit()*radius;
}

/**
 * Function: spinUpAroundCenter
 *
 * This spins you in the up direction, from the perspective of the
 * viewer.
 *
 * Params:
 *   angle - the angle in degrees that you wish to spin.
 */
void Camera::spinUpAroundCenter(GLdouble angle) {
    double radius = (eyePosition - lookAtPosition).getMag();

    //Set up some basis vectors for the plane in which you want to move
    Point3 basisY = (eyePosition - lookAtPosition).getUnit();
    Point3 basisX = Point3::cross(basisY, upDirection).getUnit();
    Point3 basisZ = Point3::cross(basisX, basisY).getUnit();

    //Rotate the basis vectors by angle, this is the vector to translate
    //to our final destination
    Point3 returnVector = basisX*cos((90-angle)*(PI/180.0)) + basisY*sin((90-angle)*(PI/180.0));

    //Translate to the final position
    eyePosition = lookAtPosition + returnVector.getUnit()*radius;
}

/**
 * Function: spinViewAroundCenter
 *
 * This rotates the apparent view of the camera in the counter-
 * clockwise direction. Really just changing the upDirection
 * vector
 *
 * Params:
 *   angle - the angle in degrees that you wish to spin.
 */
void Camera::spinViewAroundCenter(GLdouble angle) {

}

//Pass in functors to change the progression
void Camera::spinAroundCenter(Delta_Functor * angleUp, Delta_Functor * angleLeft) {
    angleUpFunct = angleUp;
    angleLeftFunct = angleLeft;
}

//Pass in angles in radians, not degrees
void Camera::spinAroundCenter(GLdouble angleUp, GLdouble angleLeft)
{
    //First, we must translate everything so that what we are looking at lies at the origin
    Point3 offset = lookAtPosition;
    eyePosition = eyePosition - offset;
    lookAtPosition = lookAtPosition - offset;

    double radius = (eyePosition - lookAtPosition).getMag();
    double phi = acos(eyePosition.z / radius);
    double theta = atan2(eyePosition.y, eyePosition.x);

    //printf("Phi was %f, theta was %f\n", phi, theta);

    phi -= angleUp;         //Minus because of the traditional definition for polar coordinates
    if (phi <= 0.010000)    //Slightly more than 0, to avoid getting directly above/below
    {
        phi = 0.010000;
    }
    else if (phi >= PI - 0.01)   //Slightly less than Pi, to avoid getting directly above/below
    {
        phi = PI - 0.01;
    }
    theta += angleLeft;
    if (theta <= -PI)         //This one is fine to be zero
    {
        theta += 2*PI;
    }
    else if (theta >= PI)
    {
        theta -= 2*PI;
    }

    //printf("Phi is now %f, theta is %f\n", phi, theta);

    //Important note: 0 < phi < Pi and  -pi < theta < pi, (thanks to atan2)
    eyePosition.x = radius*cos(theta)*sin(phi);
    eyePosition.y = radius*sin(theta)*sin(phi);
    eyePosition.z = radius*cos(phi);

    //Now, translate everything back to how it was
    eyePosition = eyePosition + offset;
    lookAtPosition = lookAtPosition + offset;
}

void Camera::spinAroundCamera(GLdouble angleUp, GLdouble angleLeft)
{
    //First, we must translate everything so that where we are looking from lies at the origin
    Point3 offset = eyePosition;
    eyePosition = eyePosition - offset;
    lookAtPosition = lookAtPosition - offset;

    //Calculate spherical coordinates around eyePosition
    double radius = (eyePosition - lookAtPosition).getMag();
    double phi = acos(lookAtPosition.z / radius);
    double theta = atan2(lookAtPosition.y, lookAtPosition.x);

    //printf("Phi was %f, theta was %f\n", phi, theta);

    phi -= angleUp;         //Minus because of the traditional definition for polar coordinates
    if (phi <= 0.010000)    //Slightly more than 0, to avoid getting directly above/below
    {
        phi = 0.010000;
    }
    else if (phi >= PI - 0.01)   //Slightly less than Pi, to avoid getting directly above/below
    {
        phi = PI - 0.01;
    }
    theta += angleLeft;
    if (theta <= -PI)         //This one is fine to be zero
    {
        theta += 2*PI;
    }
    else if (theta >= PI)
    {
        theta -= 2*PI;
    }

    //printf("Phi is now %f, theta is %f\n", phi, theta);

    //Important note: 0 < phi < Pi and  -pi < theta < pi, (thanks to atan2)
    lookAtPosition.x = radius*cos(theta)*sin(phi);
    lookAtPosition.y = radius*sin(theta)*sin(phi);
    lookAtPosition.z = radius*cos(phi);

    //Now, translate everything back to how it was
    eyePosition = eyePosition + offset;
    lookAtPosition = lookAtPosition + offset;
}

void Camera::zoomIn(GLdouble amount)
{
    eyePosition = eyePosition * amount;
}

//Shift the camera in some direction by some amount
void Camera::shift(Point3 direction, GLdouble amount)
{
    Point3 shift = direction.getUnit()*amount;
    eyePosition = eyePosition + shift;
    lookAtPosition = lookAtPosition + shift;
}
