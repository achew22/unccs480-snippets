#ifndef CAMERA_H
#define CAMERA_H

#ifndef PI  //Defining PI this way allows us to define it wherever we need it and not worry about multiple definitions
#define PI 3.1415926
#endif

#include <GL/gl.h>

#include "point.h"
#include "DeltaFunctor.h"
#include "path.h"

/** A class that acts as a camera.
 *
 * Largely, this class is a wrapper for calls to gluLookAt(). However, it has a few
 * advantages, with more in the works. Aside from simply setting the location of
 * the camera, the location it looks at, and the direction which is 'up', this
 * class has members to rotate the camera by some angle as though it were on the
 * surface of a sphere. In the future, we will include functions to use DeltaFunctors
 * to set a path in three dimensional space for the camera to follow, allowing for
 * pre-defined complex camera motions. In addition, we will create functions which
 * are essentially the reverse of the current spherical functions, where the point that
 * you are looking at can be adjusted as though it were on the surface of a sphere
 * centered at the eye. This is similar to a FPS type camera mode.
 *
 * @todo Set up a function to use DeltaFunctors as a path for the camera.
 * @todo Create functions which move the lookAtPosition as though on the surface of a sphere.
 * @todo Build an FPS mode.
 */
class Camera
{
private:
    Point3 eyePosition; ///<The position of the camera.
    Point3 lookAtPosition; ///<The location that the camera is looking at.
    Point3 upDirection; ///<The direction which is 'up', with reference to the camera.

    Delta_Functor * angleUpFunct; ///<Deprecated, do not use.
    Delta_Functor * angleLeftFunct; ///<Deprecated, do not use.

    Path* path; ///< The path that the camera will follow.

    /** Contains the coordinates for a frustum.
     *
     * This is all of the pieces that you would put into a normal gluLookAt() call.
     * The first two elements are the left and right coordinates of the viewing plane.
     * The second two elements are the bottom and top coordinates of the viewing plane.
     * The last two elements are the distances to the near and far clipping planes.
     * Note that 0 < frustum[4] < frustum[5], else there are problems. A
     * fairly standard setup would be as follows (width and height are the width and
     * height of your window):
     *
     * frustum[] = {-width/height, width/height, -1, 1, 2, 100}
     *
     * @see setFrustum()
     */
    GLdouble frustum [6];
public:
    Camera();   ///< The default constructor for the camera class.
    Camera(Point3 eye, Point3 lookAt, Point3 up);   ///< A constructor for the camera class.
    void setEyePosition(Point3 pos);    ///< Set the position of the camera.
    void setEyePosition(double posx, double posy, double posz); ///< Set the position of the camera.
    void setLookAtPosition(Point3 lookAt);  ///< Set the location that the camera is looking at.
    void setLookAtPosition(double lookAtx, double lookAty, double lookAtz);  ///< Set the location that the camera is looking at.
    void setUpDirection(Point3 up); ///< Set the direction which is 'up', with reference to the camera.
    void setUpDirection(double upx, double upy, double upz); ///< Set the direction which is 'up', with reference to the camera.
    void setFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);   ///< Set the viewing frustum.
    void update();  ///< Update the camera.

    //New motion functions move on surface of sphere around center
    void spinRightAroundCenter(GLdouble angle); ///< Spin right or left.
    void spinUpAroundCenter(GLdouble angle);    ///< Spin up or down.
    void spinViewAroundCenter(GLdouble angle);  ///< Rotates the camera.

    //New to follow a path
    void followPath(Path pathToFollow); ///< Follow a given path (as eyePosition)
    void unFollowPath(); ///< Stop following a given path (as eyePosition)

    void zoomIn(GLdouble amount);   ///< Zoom in/out.
    void shift(Point3 direction, GLdouble amount);  ///< Shift the camera.
};

#endif //CAMERA_H
