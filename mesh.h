#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <fstream>

#include "face.h"
#include "point.h"

class Mesh
{
private:
    std::vector<Face> faces;
    std::vector<Point3> vertices;
    std::vector<Point3> textures;
    std::vector<Point3> normals;
public:
    Mesh();
    void loadObj(std::string filename);
    void addFace(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes);
    void addFace(std::vector<int>& vertIndexes);
    void addFaceAndReverse(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes);
    void addFaceAndReverse(std::vector<int>& vertIndexes);
    bool drawMesh();
};

#endif //MESH_H
