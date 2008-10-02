#include <GL/glut.h>

#include "face.h"

//Empty constructor
Face::Face()
{
}

//Pass in all of the points of the face, and how many points there are
Face::Face(Point3 inPoints[], int numPoints)
{
    points.resize(numPoints);
    for (int i = 0; i < numPoints; i++)
    {
        points[i] = inPoints[i];
    }
}

//Add a new point
void Face::addPoint(Point3 point)
{
    points.push_back(point);
}


//Add a new point
void Face::addPoint(double x, double y, double z)
{
    points.push_back(Point3(x, y, z));
}

//Draw the face to the screen
void Face::drawFace()
{
    glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < points.size(); i++)
        {
            (points[i]).doVertex();
        }
    glEnd();
}
