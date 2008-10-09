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
public:
    Mesh();
    void loadObj(std::string filename);
    void addFace(std::vector<int>& indexes);
    void addFaceAndReverse(std::vector<int>& indexes);
    bool drawMesh();
};

#endif //MESH_H
