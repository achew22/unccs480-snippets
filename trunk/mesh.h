#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <fstream>

#include "../unccs480-snippets/face.h"
#include "../unccs480-snippets/point.h"

class Mesh
{
private:
    std::vector<Face> faces;
//    std::vector<Edge> edges;
    std::vector<Point3> vertices;
public:
    Mesh();
    Mesh(std::string filename);
    void addFace(std::vector<Point3>& points);
    void drawMesh();
};

#endif //MESH_H
