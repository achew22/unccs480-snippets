#include <GL/glut.h>

#include "face.h"
#include "point.h"

//Empty constructor
Face::Face()
{
    normal = Point3(0,0,1); //Default normal - probably unnecessary
}

//Pass in all of the points of the face, and how many points there are
Face::Face(Point3 inPoints[], int numPoints)
{
    points.resize(numPoints);
    normal = Point3(0,0,1); //Default normal - probably unnecessary
    for (int i = 0; i < numPoints; i++)
    {
        points[i] = inPoints[i];
    }
    if (numPoints >= 3)
    {
        normal = ((points[1] - points[0]).cross(points[2] - points[0])).getUnit();
        //printf("Got normal: %f, %f, %f \n", normal.x, normal.y, normal.z);
    }
}

//Add a new point
void Face::addPoint(Point3 point)
{
    points.push_back(point);
    if (points.size() >= 3)
    {
        normal = ((points[1] - points[0]).cross(points[2] - points[0])).getUnit();
        //printf("Got normal: %f, %f, %f \n", normal.x, normal.y, normal.z);
    }
}


//Add a new point
void Face::addPoint(double x, double y, double z)
{
    points.push_back(Point3(x, y, z));
    if (points.size() >= 3)
    {
        normal = ((points[1] - points[0]).cross(points[2] - points[0])).getUnit();
        //printf("Got normal: %f, %f, %f \n", normal.x, normal.y, normal.z);
    }
}

//Draw the face to the screen
void Face::drawFace()
{
    glNormal3f(normal.x, normal.y, normal.z);
    glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < points.size(); i++)
        {
            (points[i]).doVertex();
        }
    glEnd();
}
