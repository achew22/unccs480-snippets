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

//Draw the face to the screen (scalar does not work yet)
void Face::drawFace(int scalar = 1)
{
    glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < points.size(); i++)
        {
            (points[i]).doVertex();
        }
    glEnd();
}
