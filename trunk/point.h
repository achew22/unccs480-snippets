#ifndef POINT_H
#define POINT_H

#include <string>
#include <GL/glut.h>

//A struct with a point in three dimensions. Allows you to peform such
//things as dot product, cross product, addition, scalar multiplication,
//and use DoVertex() to make a glVertex call.
class Point3
{
private:
	bool loadError;
public:
    GLdouble x;
    GLdouble y;
    GLdouble z;

/*    GLdouble normx;
    GLdouble normy;
    GLdouble normz;
*/

    Point3 * normal;

    Point3();
    Point3(const Point3& point);
    Point3(GLdouble inx, GLdouble iny, GLdouble inz);
    Point3(std::string pointStr); //Format "x,y,z" with/without spaces or "v 2.3 35 20.3"
    void setNormal(GLdouble inx, GLdouble iny, GLdouble inz);
    void setNormal(Point3 * toSet);
	bool getLoadError();
    Point3 operator +(const Point3& toadd);
    Point3 operator -(const Point3& tosub);
    Point3 operator *(const double& tomult);
	double operator *(const Point3& todot);
    Point3 operator /(const double& todiv);
    Point3 operator =(Point3 toequal);
    bool operator ==(Point3 toequal);
    double dot(const Point3& todot);
    Point3 cross(const Point3& tocross);
	static double dot(const Point3& a, const Point3& b);
	static Point3 cross(const Point3& a, const Point3& b);
    double getMag();
    Point3 getUnit();
    void doVertex();
    void doNormal();
};

#endif //POINT_H
