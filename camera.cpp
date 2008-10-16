#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>

#include "camera.h"
#include "error.h"

#ifndef PI
#define PI 3.1415926
#endif //PI

/**
 * By default the following are set:
 * - eyePosition = <0, 0, 1>,
 * - lookAtPosition = <0, 0, 0>,
 * - upDirection = <1, 0, 0>,
 * - frustum[] = {-1, 1, -1, 1, 2, 100},
 * - angleLeftFunct = NULL,
 * - angleUpFunct = NULL.
 */
Camera::Camera() {
    //Set up defaults
    eyePosition = Point3(0,0,1);
    lookAtPosition = Point3(0,0,0);
    upDirection = Point3(1,0,0);

    frustum[0] = -1;
    frustum[1] = 1;
    frustum[2] = -1;
    frustum[3] = 1;
    frustum[4] = 2;
    frustum[5] = 100;

    angleLeftFunct = angleUpFunct = NULL;
}

/**
 * Sets up frustum as the default:
 * frustum[] = {-1, 1, -1, 1, 2, 100}.
 * To change it, use setFrustum()
 *
 * @param pos is the position of the camera
 * @param lookAt is the location that the camera is looking at
 * @param up is the direction that is 'up', with reference to the camera
 * @see eyePosition
 * @see lookAtPosition
 * @see upDirection
 * @see frustum[]
 * @see setFrustum()
 */
Camera::Camera(Point3 pos, Point3 lookAt, Point3 up) {
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

/**
 * @param pos is the position that you wish to set the camera at.
 * @see eyePosition
 */
void Camera::setEyePosition(Point3 pos) {
    eyePosition = pos;
}

/**
 * @param posx is the x coordinate of the position that you wish to set the camera at.
 * @param posy is the y coordinate of the position that you wish to set the camera at.
 * @param posz is the z coordinate of the position that you wish to set the camera at.
 * @see eyePosition
 */
void Camera::setEyePosition(double posx, double posy, double posz) {
    eyePosition = Point3(posx, posy, posz);
}

/**
 * @param lookAt is the location that you wish to set the camera looking at
 * @see lookAtPosition
 */
void Camera::setLookAtPosition(Point3 lookAt) {
    lookAtPosition = lookAt;
}

/**
 * @param lookAtx is the x coordinate of the location that you wish to set the camera looking at
 * @param lookAty is the y coordinate of the location that you wish to set the camera looking at
 * @param lookAtz is the z coordinate of the location that you wish to set the camera looking at
 * @see lookAtPosition
 */
void Camera::setLookAtPosition(double lookAtx, double lookAty, double lookAtz) {
    lookAtPosition = Point3(lookAtx, lookAty, lookAtz);
}

/**
 * @param up is the direction vector which is 'up', with reference to the camera
 * @see upDirection
 */
void Camera::setUpDirection(Point3 up) {
    upDirection = up;
}

/**
 * @param upx is the x coordinate of the direction vector which is 'up', with reference to the camera
 * @param upy is the y coordinate of the direction vector which is 'up', with reference to the camera
 * @param upz is the z coordinate of the direction vector which is 'up', with reference to the camera
 * @see upDirection
 */
void Camera::setUpDirection(double upx, double upy, double upz) {
    upDirection = Point3(upx, upy, upz);
}

/**
 * A recommended call would be as follows (width/height are the width and height of the window):
 *
 * myCamera->setFrustum(-width/height, width/height, -1, 1, 2, 100);
 *
 * @param left is the left edge of the viewing frustum
 * @param right is the right edge of the viewing frustum
 * @param bottom is the bottom edge of the viewing frustum
 * @param top is the top edge of the viewing frustum
 * @param zNear is the distance to the nearest clipping plane, must be greater than 0
 * @param zFar is the distance to the farthest clipping plane, must be greater than zNear
 * @see frustum[]
 */
void Camera::setFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
    frustum[0] = left;
    frustum[1] = right;
    frustum[2] = bottom;
    frustum[3] = top;
    frustum[4] = zNear;
    frustum[5] = zFar;
}

/**
 * This function updates the projection matrix so that it includes the camera at
 * its current position, direction, etc. Should be called before each frame is rendered.
 */
void Camera::update() {
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
 * SpinRightAroundCenter moves (as you would
 * expect) to the right, from the perspective of the viewer, a
 * though the camera were on the surface of a sphere centered at
 * lookAtPosition. Makes no changes to lookAtPosition or upDirection, but does
 * adjust eyePosition. Positive values of angle move right, negative
 * move left.
 *
 * This function is a part of the
 * spin***AroundCenter() group of functions in which motion
 * acts as though the camera is on the surface of a sphere centered
 * at lookAtPosition.
 *
 * @param angle is the angle in degrees that you wish to rotate by
 * @see lookAtPosition
 * @see upDirection
 * @see eyePosition
 * @see spinUpAroundCenter()
 * @see spinViewAroundCenter()
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
 * This spins you in the up direction, from the perspective of the
 * viewer, as though on the surface of a sphere centered at lookAtPosition.
 * This function will not change lookAtPosition, but will adjust both
 * upDirection and eyePosition. Positive values of angle move up, negative
 * values move down.
 *
 * This function is a part of the
 * spin***AroundCenter() group of functions in which motion
 * acts as though the camera is on the surface of a sphere centered
 * at lookAtPosition.
 *
 * @param angle is the angle in degrees that you wish to spin.
 * @see lookAtPosition
 * @see upDirection
 * @see eyePosition
 * @see spinRightAroundCenter()
 * @see spinViewAroundCenter()
 */
void Camera::spinUpAroundCenter(GLdouble angle) {
    double radius = (eyePosition - lookAtPosition).getMag();

    //Set up some basis vectors for the plane in which you want to move
    Point3 basisY = (eyePosition - lookAtPosition).getUnit();
    Point3 basisX = Point3::cross(basisY, upDirection).getUnit();
    Point3 basisZ = Point3::cross(basisX, basisY).getUnit();

    //Rotate the basis vectors by angle, this is the vector to translate
    //to our final destination
    Point3 returnVector = basisY*cos((angle)*(PI/180.0)) + basisZ*sin((angle)*(PI/180.0));

    //Adjust the upDirection
    upDirection = basisZ;

    //Translate to the final position
    eyePosition = lookAtPosition + returnVector.getUnit()*radius;
}

/**
 * This rotates the apparent view of the camera in the counter-
 * clockwise direction. Really just changing the upDirection
 * vector, makes no changes to lookAtPosition or eyePosition.
 * Positive values of angle are CCW, negative are CW.
 *
 * This function is a part of the
 * spin***AroundCenter() group of functions in which motion
 * acts as though the camera is on the surface of a sphere centered
 * at lookAtPosition.
 *
 * @param angle is The angle in degrees you wish to spin
 * @see lookAtPosition
 * @see upDirection
 * @see eyePosition
 * @see spinRightAroundCenter()
 * @see spinUpAroundCenter()
 */
void Camera::spinViewAroundCenter(GLdouble angle) {
    //Set up some basis vectors for the plane in which you want to move
    Point3 basisY = (eyePosition - lookAtPosition).getUnit();
    Point3 basisX = Point3::cross(basisY, upDirection).getUnit();
    Point3 basisZ = Point3::cross(basisX, basisY).getUnit();

    //Adjust the upDirection
    upDirection = basisX*cos((90+angle)*(PI/180.0)) + basisZ*sin((90+angle)*(PI/180.0));
}

/**
 * Zoom the camera in or out, depending on value of amount.
 * Values less than one will zoom in, values greater than one
 * will zoom out. Reciprocals will zoom in/out equal amounts,
 * so that zoomIn(3) followed by zoomIn(1.0/3.0) will result
 * in no movement at all. This function only adjusts the value
 * of eyePosition.
 *
 * @param amount is the zoom scalar
 * @see eyePosition
 */
void Camera::zoomIn(GLdouble amount) {
    eyePosition = eyePosition * amount;
}

/**
 * Shift the camera in some direction by some amount. Does not
 * change the direction that you are looking, or the upDirection.
 * Will, however, change eyePosition and lookAtPosition.
 *
 * @param direction is a vector pointing in the direction of the shift
 * @param amount is the distance of the shift
 * @see lookAtPosition
 * @see upDirection
 * @see eyePosition
 */
void Camera::shift(Point3 direction, GLdouble amount) {
    Point3 shift = direction.getUnit()*amount;
    eyePosition = eyePosition + shift;
    lookAtPosition = lookAtPosition + shift;
}
