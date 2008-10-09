#ifndef FACE_H
#define FACE_H

#include <vector>

#include "point.h"

//A class for creating a generic 2dimensional face. Pass in the points through
//the constructor, then use the DrawFace command to draw it to the screen
class Face
{
private:
    std::vector<Point3>* points;
    std::vector<int> indexes;
    bool isFan; //True if we should use triangleFan, false if we should use triangleStrip
public:
    Face();
    void registerPoints(std::vector<Point3>* toRegister);
    void registerIndexes(std::vector<int>& toRegister);
    void addIndex(int index);
    void makeFan();
    void makeStrip();
    void addNormalToPoints();
    bool drawFace();
};

#endif //FACE_H
