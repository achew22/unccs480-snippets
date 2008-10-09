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
//    std::vector<Edge> edges;
    std::vector<Point3> vertices;
public:
    Mesh();
    Mesh(std::string filename);
    void loadObj(std::string filename);
    void addFace(std::vector<Point3>& points);
    void addFaceAndReverse(std::vector<Point3>& points);
    bool drawMesh();
};

#endif //MESH_H
