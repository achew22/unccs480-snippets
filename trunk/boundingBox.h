#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "point.h"
#include "mesh.h"

struct boundingBoxNode{
public:
    Point3 center;
    double radius;
    boundingBoxNode* left;
    boundingBoxNode* right;

    boundingBoxNode() {
        center = Point3(0,0,0);
        radius = 0;
        left = right = NULL;
    }
    ~boundingBoxNode() {
        if (left != NULL) {
            delete left;
        }
        if (right != NULL) {
            delete right;
        }
    }
};

class boundingBox{
private:
    boundingBoxNode* head;
    double precision;
    int depth;
public:
    boundingBox() {
        head = NULL;
    }
    ~boundingBox() {
        if (head != NULL) {
            delete head;
        }
    }
    void calculateFromMesh(Mesh* mesh);
    void calculateFromMesh(Mesh* mesh, double precisionToSet, int depthToSet);
};

#endif //BOUNDINGBOX_H
