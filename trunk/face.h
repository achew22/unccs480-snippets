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
    Face(); ///< Default constructor.
    void registerVertices(std::vector<Point3>* toRegister); ///< Register the vector of vertices to use.
    void registerTextures(std::vector<Point2>* toRegister); ///< Register the vector of texturePts to use.
    void registerNormals(std::vector<Point3>* toRegister);  ///< Register the vector of normals to use.
    void registerVertexIndexes(std::vector<int>& toRegister);   ///< Register the vector of indexes of vertices to use.
    void registerTextureIndexes(std::vector<int>& toRegister);  ///< Register the vector of indexes of texturePts to use.
    void registerNormalIndexes(std::vector<int>& toRegister);   ///< Register the vector of indexes of normals to use.
    void addVertexIndex(int index);     ///< Add a vertex index.
    void addTextureIndex(int index);    ///< Add a texturePts index.
    void addNormalIndex(int index);     ///< Add a normal index.
    void makeFan();     ///< Set isFan to true.
    void makeStrip();   ///< Set isFan to false.
    void addOwnNormals();   ///< Used to calculate normals for a mesh.
    bool drawFace();    ///< Draw the face to the screen.
};

#endif //FACE_H
