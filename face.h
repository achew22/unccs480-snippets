#ifndef FACE_H
#define FACE_H

#include <vector>

#include "point.h"

//A class for creating a generic 2dimensional face. Pass in the points through
//the constructor, then use the DrawFace command to draw it to the screen
class Face
{
private:
    std::vector<Point3> points;
public:
    Face();
    Face(Point3 inPoints[], int numPoints);
    void addPoint(Point3 point);
    void addPoint(double x, double y, double z);
    void drawFace();
};

#endif //FACE_H
