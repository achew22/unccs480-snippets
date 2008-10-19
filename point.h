#ifndef POINT_H
#define POINT_H

#include <string>
#include <GL/gl.h>

/** A three-dimensional point.
 *
 * This class can be used to interchangeably represent either a point in
 * three-dimensional space or a vector in three-dimensional space. It
 * includes many of the functions that one would expect for such things,
 * including addition, subtraction, scalar multiplication, dot product,
 * cross product, unit vector calculation, and magnitude calculation.
 */
class Point3
{
private:
	bool loadError; ///< True if there is a loading error.
public:
    GLdouble x; ///< The x coordinate
    GLdouble y; ///< The y coordinate
    GLdouble z; ///< The z coordinate

    Point3();   ///< Default constructor.
    Point3(const Point3& point);    ///< Another constructor.
    Point3(GLdouble inx, GLdouble iny, GLdouble inz);   ///< Usual constructor.
    Point3(std::string pointStr); ///< Special constructor.
	bool getLoadError();    ///< Find out if there was a construction error.
    Point3 operator +(const Point3& toadd); ///< Add two points as vectors.
    Point3 operator -(const Point3& tosub); ///< Subtract two points as vectors.
    Point3 operator *(const double& tomult);    ///< Multiply a vector by a scalar.
	double operator *(const Point3& todot); ///< Dot two vectors.
    Point3 operator /(const double& todiv); ///< Divide a vector by a scalar.
    Point3 operator =(Point3 toequal);  ///< Assign the value of a vector to this one.
    bool operator ==(Point3 toequal);   ///< Determine if two vectors are equal.
    double dot(const Point3& todot);    ///< Dot this vector with another.
    Point3 cross(const Point3& tocross);    ///< Cross this vector with another.
	static double dot(const Point3& a, const Point3& b);    ///< Static function to dot two vectors.
	static Point3 cross(const Point3& a, const Point3& b);  ///< Static function to cross two vectors.
    double getMag();    ///< Get the magnitude of this vector.
    Point3 getUnit();   ///< Get a unit vector pointing in the direction of this vector.
    void doVertex();    ///< Perform a glVertex3d() call.
    void doNormal();    ///< Perform a glNormal3d() call.
};

/** A two-dimensional point.
 *
 * This class can be used to interchangeably represent either a point in
 * two-dimensional space or a vector in two-dimensional space. It
 * includes many of the functions that one would expect for such things,
 * including addition, subtraction, scalar multiplication, dot product,
 * unit vector calculation, and magnitude calculation.
 */
class Point2 {
private:
    bool loadError; ///< True if there is a loading error.
public:
    GLdouble x; ///< The x coordinate
    GLdouble y; ///< The y coordinate

    Point2();   ///< Default constructor.
    Point2(const Point2& point);    ///< Another constructor.
    Point2(GLdouble inx, GLdouble iny);   ///< Usual constructor.
    Point2(std::string pointStr); ///< Special constructor.
	bool getLoadError();    ///< Find out if there was a construction error.
    Point2 operator +(const Point2& toadd); ///< Add two points as vectors.
    Point2 operator -(const Point2& tosub); ///< Subtract two points as vectors.
    Point2 operator *(const double& tomult);    ///< Multiply a vector by a scalar.
	double operator *(const Point2& todot); ///< Dot two vectors.
    Point2 operator /(const double& todiv); ///< Divide a vector by a scalar.
    Point2 operator =(Point2 toequal);  ///< Assign the value of a vector to this one.
    bool operator ==(Point2 toequal);   ///< Determine if two vectors are equal.
    double dot(const Point2& todot);    ///< Dot this vector with another.
	static double dot(const Point2& a, const Point2& b);    ///< Static function to dot two vectors.
    double getMag();    ///< Get the magnitude of this vector.
    Point2 getUnit();   ///< Get a unit vector pointing in the direction of this vector.
    void doTex();   ///< Peform a glTexCoord2f() call.
};

#endif //POINT_H
