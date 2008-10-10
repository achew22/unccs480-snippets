#include <cmath>
#include <string>
#include <stdio.h>

#include <GL/glut.h>

#include "point.h"

//Default constructor
Point3::Point3()
{
    x = 0;
    y = 0;
    z = 0;

/*
    normx = 0;
    normy = 0;
    normz = 0;
*/
	loadError = false;
}

//Construct from another point
Point3::Point3(const Point3& point)
{
    x = point.x;
    y = point.y;
    z = point.z;
/*
    normx = 0;
    normy = 0;
    normz = 0;
*/
	loadError = false;
}

//Construct from coordinates
Point3::Point3(GLdouble inx, GLdouble iny, GLdouble inz)
{
    x = inx;
    y = iny;
    z = inz;
/*
    normx = 0;
    normy = 0;
    normz = 0;
*/
	loadError = false;
}

void Point3::setNormal(GLdouble inx, GLdouble iny, GLdouble inz) {
    normal = new Point3(inx, iny, inz);
}

void Point3::setNormal(Point3 * toSet) {
    normal = toSet;
}
//Construct from string of form "x,y,z" where they are seperated by commas
//or from the form in .obj files, which is "v 2.322 35.6 22.334"
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
        if (sscanf(pointStr.c_str(), "v %f %f %f", &a, &b, &c) == 3) {
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

//If there was an error in loading the Point (specifically when you load from a string), this will return true. Otherwise false
bool Point3::getLoadError()
{
	return loadError;
}

//Add two points/vectors
Point3 Point3::operator +(const Point3& toadd)
{
    return Point3(x + toadd.x, y + toadd.y, z + toadd.z);
}

//Subtract two points/vectors
Point3 Point3::operator -(const Point3& tosub)
{
    return Point3(x - tosub.x, y - tosub.y, z - tosub.z);
}

//Multiply a vector by a constant
Point3 Point3::operator *(const double& tomult)
{
    return Point3(x * tomult, y * tomult, z * tomult);
}

//Dot two vectors
double Point3::operator *(const Point3& todot)
{
	return x*todot.x + y*todot.y + z*todot.z;
}

//Divide by a constant
Point3 Point3::operator /(const double& todiv)
{
    if (todiv == 0)
    {
        return *this;
    }
    return Point3(x / todiv, y / todiv, z / todiv);
}

//Set a point equal to another point
Point3 Point3::operator =(Point3 toequal)
{
    x = toequal.x;
    y = toequal.y;
    z = toequal.z;
    return toequal;
}

//Determine if two points are equal
bool Point3::operator ==(Point3 toequal)
{
    return (x == toequal.x && y == toequal.y && z == toequal.z);
}

//Dot two vectors and return the dot product
double Point3::dot(const Point3& todot)
{
    return x*todot.x + y*todot.y + z*todot.z;
}

//Return the cross product of two vectors
Point3 Point3::cross(const Point3& tocross)
{
    return Point3((y*tocross.z - z*tocross.y), -(x*tocross.z - z*tocross.x), (x*tocross.y - y*tocross.x));
}

//Dot two vectors
static double dot(const Point3& a, const Point3& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

//Cross two vectors
static Point3 cross(const Point3& a, const Point3& b)
{
	return Point3((a.y*b.z - a.z*b.y), -(a.x*b.z - a.z*b.x), (a.x*b.y - a.y*b.x));
}

//Get the magnitude of a given vector
double Point3::getMag()
{
    return sqrt(x*x + y*y + z*z);
}

//Return the unit version of this vector
Point3 Point3::getUnit()
{
    return *this/getMag();
}

//Perform a glVertex3d call
void Point3::doVertex()
{
    glVertex3d(x,y,z);
}

//Perform a glNormal3d call
void Point3::doNormal() {
    if (normal) {
        glNormal3d(normal->x, normal->y, normal->z);
    }
    else {
        glNormal3d(0, 0, 0);
    }
}

