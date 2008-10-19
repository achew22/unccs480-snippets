#ifndef FACE_H
#define FACE_H

#include <vector>

#include "point.h"

//A class for creating a generic 2dimensional face. Pass in the points through
//the constructor, then use the DrawFace command to draw it to the screen
class Face
{
private:
    std::vector<Point3>* vertices;
    std::vector<Point2>* textures;
    std::vector<Point3>* normals;
    std::vector<int> vertexIndex;
    std::vector<int> textureIndex;
    std::vector<int> normalIndex;
    bool isFan; //True if we should use triangleFan, false if we should use triangleStrip
public:
    Face();
    void registerVertices(std::vector<Point3>* toRegister);
    void registerTextures(std::vector<Point2>* toRegister);
    void registerNormals(std::vector<Point3>* toRegister);
    void registerVertexIndexes(std::vector<int>& toRegister);
    void registerTextureIndexes(std::vector<int>& toRegister);
    void registerNormalIndexes(std::vector<int>& toRegister);
    void addVertexIndex(int index);
    void addTextureIndex(int index);
    void addNormalIndex(int index);
    void makeFan();
    void makeStrip();
    void addOwnNormals();
    bool drawFace();
};

#endif //FACE_H
