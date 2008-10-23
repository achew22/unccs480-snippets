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
    std::vector<Point2> texturePts;
    std::vector<Point3> normals;
    int textureId;
public:
    Mesh();
    void loadObj(std::string filename);
    void loadTexture(std::string filename);
    void addFaceVTN(std::vector<int>& vertIndexes, std::vector<int>& textIndexes, std::vector<int>& normalIndexes);
    void addFaceVN(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes);
    void addFaceVT(std::vector<int>& vertIndexes, std::vector<int>& textIndexes);
    void addFaceV(std::vector<int>& vertIndexes);
    void addFaceAndReverseVTN(std::vector<int>& vertIndexes, std::vector<int>& textIndexes, std::vector<int>& normalIndexes);
    void addFaceAndReverseVN(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes);
    void addFaceAndReverseVT(std::vector<int>& vertIndexes, std::vector<int>& textIndexes);
    void addFaceAndReverseV(std::vector<int>& vertIndexes);
    void calculateNormals();
    Point3 calculateCenter();
    double calculateAverageRadius();
    bool drawMesh();
};

#endif //MESH_H
