#include <cmath>
#include <string>
#include <stdio.h>

#include <GL/gl.h>

#include "point.h"

/**
 * All coordinates default to zero.
 */
Point3::Point3()
{
    x = 0;
    y = 0;
    z = 0;

	loadError = false;
}

/**
 * Standard constructor from an existing Point3.
 *
 * @param point is the point that the created point will be equal to.
 * /
Point3::Point3(const Point3& point)
{
    x = point.x;
    y = point.y;
    z = point.z;

	loadError = false;
}

/**
 * Standard constructor which is used most often. Pass in the
 * coordinates of the point to construct.
 *
 * @param inx is the x coordinate of the point to construct.
 * @param iny is the y coordinate of the point to construct.
 * @param inz is the z coordinate of the point to construct.
 */
Point3::Point3(GLdouble inx, GLdouble iny, GLdouble inz)
{
    x = inx;
    y = iny;
    z = inz;

	loadError = false;
}

/**
 * This constructor is only used in the Mesh class.
 *
 * It takes in a string of a specific type, usually the type that
 * is given in standard .obj files. This string is of the form
 * "v* n1 n2 n3", where '*' is a character (or nothing), and n1, n2,
 * and n3 are all doulbles. If pointStr is not of the proper format,
 * then it throws an error to the error class and sets loadError
 * to true, then uses the default constructor.
 *
 * @param pointStr is the string to turn into a point.
 */
Point3::Point3(std::string pointStr)
{
    if (pointStr.find_first_of(',') != std::string::npos) {    //This means that it is a comma delimeted type
        //First, remove all spaces
        //printf("Point3::Removing all spaces from %s...", pointStr.c_str());
        int pos = 0;
        loadError = false;
        while ((pos = pointStr.find_first_of(' ')) != std::string::npos)
        {
            pointStr.erase(pos,1);
        }
        //printf("Done\n");

        //Now get the values
        //printf("Point3::Getting values from %s...", pointStr.c_str());
        float a,b,c;
        if (sscanf(pointStr.c_str(),"%f,%f,%f",&a,&b,&c) == 3)
        {
            //printf("FoundThem...");
            x = a;
            y = b;
            z = c;
        }
        else
        {
            //printf("Didn't Find them, sscanf = %i...", sscanf(pointStr.c_str(),"%d %*c %d %*c %d",&a,&b,&c));
            x = y = z = 0;
            loadError = true;
        }
        //printf("Done\n");
    } else if (pointStr.size() > 0 && pointStr[0] == 'v') {     //This would indicate a .obj style vertex
        //printf("Point3::Getting values from %s...", pointStr.c_str());
        float a,b,c;
        if (sscanf(pointStr.c_str(), "%*s %f %f %f", &a, &b, &c) == 3) {
            //printf("FoundThem...");
            x = a;
            y = b;
            z = c;
        } else {
            printf("Point3::Error didn't find them, sscanf = %i...", sscanf(pointStr.c_str(), "v %f %f %f", &a, &b, &c));
            x = y = z = 0;
            loadError = true;
        }
    } else {    //This would indicate an error in formatting
        printf("Point3: Error, string format not recognized\n");
        loadError = true;
    }
}

/**
 * If there was an error loading the point, as could occur when you load it
 * from a string.
 *
 * @return If there was a load error, returns true. Else returns false.
 */
bool Point3::getLoadError()
{
	return loadError;
}

/**
 * This function does not edit the value of *this.
 *
 * @param toadd is the vector to be added to this one.
 * @return returns the sum of these two vectors.
 */
Point3 Point3::operator +(const Point3& toadd)
{
    return Point3(x + toadd.x, y + toadd.y, z + toadd.z);
}

/**
 * This function does not edit the value of *this.
 *
 * @param tosub is the vector to be subtracted from this one.
 * @return returns the difference of these two vectors.
 */
Point3 Point3::operator -(const Point3& tosub)
{
    return Point3(x - tosub.x, y - tosub.y, z - tosub.z);
}

/**
 * This function does not edit the value of *this.
 *
 * @param tomult is the scalar to multiply this vector by.
 * @return returns the scalar product of this and tomult.
 */
Point3 Point3::operator *(const double& tomult)
{
    return Point3(x * tomult, y * tomult, z * tomult);
}

/**
 * The dot product is calculated as the traditional dot
 * product of two vectors in three-dimensional space.
 *
 * This function does not edit the value of *this.
 *
 * @param todot is the vector to be dotted with this one.
 * @return returns the dot product of these two vectors.
 */
double Point3::operator *(const Point3& todot)
{
	return x*todot.x + y*todot.y + z*todot.z;
}

/**
 * This function does not edit the value of *this.
 *
 * @param todiv is the scalar to divide this vector by (cannot be zero).
 * @return returns the scalar product of this vector and the reciprocal of todiv.
 */
Point3 Point3::operator /(const double& todiv)
{
    if (todiv == 0)
    {
        return *this;
    }
    return Point3(x / todiv, y / todiv, z / todiv);
}

/**
 * @param toequal is the vector which this one should be set equal to.
 * @return returns this vector after it is reassigned.
 */
Point3 Point3::operator =(Point3 toequal)
{
    x = toequal.x;
    y = toequal.y;
    z = toequal.z;
    return toequal;
}

/**
 * This function does not edit the value of *this.
 *
 * @param toequal is the vector to compare to this one.
 * @return returns true if the toEqual and this vector are the same, false otherwise.
 */
bool Point3::operator ==(Point3 toequal)
{
    return (x == toequal.x && y == toequal.y && z == toequal.z);
}

/**
 * The dot product is calculated as the traditional dot
 * product of two vectors in three-dimensional space.
 *
 * This function does not edit the value of *this.
 *
 * @param todot is the vector to be dotted with this one.
 * @return returns the dot product of these two vectors.
 */
double Point3::dot(const Point3& todot)
{
    return x*todot.x + y*todot.y + z*todot.z;
}

/**
 * The cross product is calculated as the traditional cross
 * product of two vectors in three-dimensional space. One
 * possible application for this is that the cross product
 * of two vectors which are not parallel is a vector perpendicular
 * to both of the originals, oriented using the right-hand-rule.
 * Note that for cross products, the order in which two vectors are
 * crossed can affect the result.
 *
 * This function does not edit the value of *this.
 *
 * @param tocross is the vector to be crossed with this one.
 * @return returns the cross product of these two vectors.
 */
Point3 Point3::cross(const Point3& tocross)
{
    return Point3((y*tocross.z - z*tocross.y), -(x*tocross.z - z*tocross.x), (x*tocross.y - y*tocross.x));
}

/**
 * The dot product is calculated as the traditional dot
 * product of two vectors in three-dimensional space. This function
 * is static because it is usually clearer to use
 * Point3::dot(a,b) than to use a.dot(b).
 *
 * @param a is the first vector to be dotted.
 * @param b is the second vector to be dotted.
 * @return returns the dot product of a and b.
 */
extern double Point3::dot(const Point3& a, const Point3& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

/**
 * The cross product is calculated as the traditional cross
 * product of two vectors in three-dimensional space. One
 * possible application for this is that the cross product
 * of two vectors which are not parallel is a vector perpendicular
 * to both of the originals, oriented using the right-hand-rule.
 * Note that for cross products, the order in which two vectors are
 * crossed can affect the result.
 *
 * @param a is the first vector to be crossed.
 * @param b is the second vector to be crossed.
 * @return returns the cross product a x b.
 */
extern Point3 Point3::cross(const Point3& a, const Point3& b)
{
	return Point3((a.y*b.z - a.z*b.y), -(a.x*b.z - a.z*b.x), (a.x*b.y - a.y*b.x));
}

/**
 * This function does not edit the value of *this.
 *
 * @return returns the magnitude of this vector.
 */
double Point3::getMag()
{
    return sqrt(x*x + y*y + z*z);
}

/**
 * This function is useful because the square root function is relatively
 * expensive. For comparison purposes, this works much faster.
 *
 * This function does not edit the value of *this.
 *
 * @return returns the magnitude of this vector squared.
 */
double Point3::getMagSquared()
{
    return x*x + y*y + z*z;
}

/**
 * This function does not edit the value of *this.
 *
 * @return returns a unit vector in the direction of *this.
 */
Point3 Point3::getUnit()
{
    return *this/getMag();
}

/**
 * Simply a nice interface so that one can store vertices of
 * an OpenGL polygon or mesh in Point3 classes. Use this
 * function in order to perform a glVertex3d() call with
 * the coordinates of this point as the arguments.
 */
void Point3::doVertex()
{
    glVertex3d(x,y,z);
}

/**
 * Simply a nice interface so that one can store normals of
 * an OpenGL polygon or mesh in Point3 classes. Use this
 * function in order to perform a glNormal3d() call with
 * the coordinates of this point as the arguments.
 */
void Point3::doNormal() {
    glNormal3d(x,y,z);
}

//********************************************************************************

//********************************Point2 Below this line**************************

//********************************************************************************

/**
 * All coordinates default to zero.
 */
Point2::Point2()
{
    x = 0;
    y = 0;

	loadError = false;
}

/**
 * Standard constructor from an existing Point2.
 *
 * @param point is the point that the created point will be equal to.
 */
Point2::Point2(const Point2& point)
{
    x = point.x;
    y = point.y;

	loadError = false;
}

/**
 * Standard constructor which is used most often. Pass in the
 * coordinates of the point to construct.
 *
 * @param inx is the x coordinate of the point to construct.
 * @param iny is the y coordinate of the point to construct.
 */
Point2::Point2(GLdouble inx, GLdouble iny)
{
    x = inx;
    y = iny;

	loadError = false;
}

/**
 * This constructor is only used in the Mesh class.
 *
 * It takes in a string of a specific type, usually the type that
 * is given in standard .obj files. This string is of the form
 * "vn* n1 n2", where '*' is a character (or nothing), and n1, n2,
 * are both doubles. If pointStr is not of the proper format,
 * then it throws an error to the error class and sets loadError
 * to true, then uses the default constructor.
 *
 * @param pointStr is the string to turn into a point.
 */
Point2::Point2(std::string pointStr)
{
    if (pointStr.size() > 0 && pointStr[0] == 'v') {     //This would indicate a .obj style vertex
        //printf("Point2::Getting values from %s...", pointStr.c_str());
        float a,b;
        if (sscanf(pointStr.c_str(), "%*s %f %f", &a, &b) == 2) {
            //printf("FoundThem...");
            x = a;
            y = b;
        } else {
            printf("Point2::Error didn't find them, sscanf = %i...", sscanf(pointStr.c_str(), "v %f %f", &a, &b));
            x = y = 0;
            loadError = true;
        }
    } else {    //This would indicate an error in formatting
        printf("Point2: Error, string format not recognized\n");
        loadError = true;
    }
}

/**
 * If there was an error loading the point, as could occur when you load it
 * from a string.
 *
 * @return If there was a load error, returns true. Else returns false.
 */
bool Point2::getLoadError()
{
	return loadError;
}

/**
 * This function does not edit the value of *this.
 *
 * @param toadd is the vector to be added to this one.
 * @return returns the sum of these two vectors.
 */
Point2 Point2::operator +(const Point2& toadd)
{
    return Point2(x + toadd.x, y + toadd.y);
}

/**
 * This function does not edit the value of *this.
 *
 * @param tosub is the vector to be subtracted from this one.
 * @return returns the difference of these two vectors.
 */
Point2 Point2::operator -(const Point2& tosub)
{
    return Point2(x - tosub.x, y - tosub.y);
}

/**
 * This function does not edit the value of *this.
 *
 * @param tomult is the scalar to multiply this vector by.
 * @return returns the scalar product of this and tomult.
 */
Point2 Point2::operator *(const double& tomult)
{
    return Point2(x * tomult, y * tomult);
}

/**
 * The dot product is calculated as the traditional dot
 * product of two vectors in three-dimensional space.
 *
 * This function does not edit the value of *this.
 *
 * @param todot is the vector to be dotted with this one.
 * @return returns the dot product of these two vectors.
 */
double Point2::operator *(const Point2& todot)
{
	return x*todot.x + y*todot.y;
}

/**
 * This function does not edit the value of *this.
 *
 * @param todiv is the scalar to divide this vector by (cannot be zero).
 * @return returns the scalar product of this vector and the reciprocal of todiv.
 */
Point2 Point2::operator /(const double& todiv)
{
    if (todiv == 0)
    {
        return *this;
    }
    return Point2(x / todiv, y / todiv);
}

/**
 * @param toequal is the vector which this one should be set equal to.
 * @return returns this vector after it is reassigned.
 */
Point2 Point2::operator =(Point2 toequal)
{
    x = toequal.x;
    y = toequal.y;
    return toequal;
}

/**
 * This function does not edit the value of *this.
 *
 * @param toequal is the vector to compare to this one.
 * @return returns true if the toEqual and this vector are the same, false otherwise.
 */
bool Point2::operator ==(Point2 toequal)
{
    return (x == toequal.x && y == toequal.y);
}

/**
 * The dot product is calculated as the traditional dot
 * product of two vectors in two-dimensional space.
 *
 * This function does not edit the value of *this.
 *
 * @param todot is the vector to be dotted with this one.
 * @return returns the dot product of these two vectors.
 */
double Point2::dot(const Point2& todot)
{
    return x*todot.x + y*todot.y;
}

/**
 * The dot product is calculated as the traditional dot
 * product of two vectors in two-dimensional space. This function
 * is static because it is usually clearer to use
 * Point2::dot(a,b) than to use a.dot(b).
 *
 * @param a is the first vector to be dotted.
 * @param b is the second vector to be dotted.
 * @return returns the dot product of a and b.
 */
extern double Point2::dot(const Point2& a, const Point2& b)
{
	return a.x*b.x + a.y*b.y;
}

/**
 * This function does not edit the value of *this.
 *
 * @return returns the magnitude of this vector.
 */
double Point2::getMag()
{
    return sqrt(x*x + y*y);
}

/**
 * This function is useful because the square root function is relatively
 * expensive. For comparison purposes, this works much faster.
 *
 * This function does not edit the value of *this.
 *
 * @return returns the magnitude of this vector squared.
 */
double Point2::getMagSquared()
{
    return x*x + y*y;
}

/**
 * This function does not edit the value of *this.
 *
 * @return returns a unit vector in the direction of *this.
 */
Point2 Point2::getUnit()
{
    return *this/getMag();
}

/**
 * This function simply performs a glTexCoord2f() call with
 * the values stored in the point.
 */
void Point2::doTex() {
    glTexCoord2f(x,y);
}
