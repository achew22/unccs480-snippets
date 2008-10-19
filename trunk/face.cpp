#include <GL/gl.h>

#include "face.h"
#include "point.h"

//Empty constructor
Face::Face() {
    vertices = NULL;
    normals = NULL;
    isFan = true;
}

void Face::registerVertices(std::vector<Point3>* toRegister) {
    vertices = toRegister;
}

void Face::registerTextures(std::vector<Point2>* toRegister) {
    textures = toRegister;
}

void Face::registerNormals(std::vector<Point3>* toRegister) {
    normals = toRegister;
}

void Face::registerVertexIndexes(std::vector<int> &toRegister) {
    vertexIndex = toRegister;
}

void Face::registerTextureIndexes(std::vector<int> &toRegister) {
    textureIndex = toRegister;
}

void Face::registerNormalIndexes(std::vector<int> &toRegister) {
    normalIndex = toRegister;
}

void Face::addVertexIndex(int index) {
    vertexIndex.push_back(index);
}

void Face::addTextureIndex(int index) {
    textureIndex.push_back(index);
}

void Face::addNormalIndex(int index) {
    normalIndex.push_back(index);
}

void Face::makeFan() {
    isFan = true;
}

void Face::makeStrip() {
    isFan = false;
}

/**
 * This function is to be called by Mesh::calculateNormals
 * Warning: will cause horrible loss of normal data if called independently
 * It adds the face's normal to each of the vertices of the face,
 * when called by all faces in a mesh, the result is proper normals.
 */
void Face::addOwnNormals() {
    //Check that there are some vertices to do this on, otherwise there is an error
    if (vertexIndex.size() == 0) {
        return;
    }

    //Set all of the normalIndexes to the same things as the vertexIndex, so that each vertex has its own normal
    normalIndex = vertexIndex;

    //Different methods if it is a fan or strip
    if (isFan) {
        //Put in the new normals for this face here
        std::vector<Point3> thisFaceNormals;
        thisFaceNormals.resize(normalIndex.size());

        //This is the base of the fan
        Point3 root = (*vertices)[vertexIndex[0]];

        //Go through each triangle, calculate the normal, then add it to all of the points of that
        for (int i = 1; i < vertexIndex.size() - 1; i++) {
            Point3 thisNormal = Point3::cross(((*vertices)[vertexIndex[i + 1]] - root), ((*vertices)[vertexIndex[i]] - root)).getUnit();
            thisFaceNormals[0] = thisFaceNormals[0] + thisNormal;
            thisFaceNormals[i+1] = thisFaceNormals[i+1] + thisNormal;
            thisFaceNormals[i] = thisFaceNormals[i] + thisNormal;
        }

        //Now add each of these face normals to the actual stack of normals
        for (int i = 0; i < thisFaceNormals.size(); i++) {
            //printf("Trying to check %i, normalIndex %i\n", i, normalIndex[i]);
            (*normals)[normalIndex[i]] = (*normals)[normalIndex[i]] + thisFaceNormals[i];
        }
    } else {
        //Put in the new normals for this face here
        std::vector<Point3> thisFaceNormals;
        thisFaceNormals.resize(normalIndex.size());

        //Go through each triangle, calculate the normal, then add it to all of the points of that
        for (int i = 0; i < vertexIndex.size() - 2; i++) {
            Point3 thisNormal;
            //Orientation is different depending on whether i is even or odd
            if (i %1 == 0) {
                thisNormal = Point3::cross(((*vertices)[vertexIndex[i + 1]] - (*vertices)[vertexIndex[i]]), ((*vertices)[vertexIndex[i + 2]] - (*vertices)[vertexIndex[i]])).getUnit();
            } else {
                thisNormal = Point3::cross(((*vertices)[vertexIndex[i + 2]] - (*vertices)[vertexIndex[i]]), ((*vertices)[vertexIndex[i + 1]] - (*vertices)[vertexIndex[i]])).getUnit();
            }
            thisFaceNormals[i] = thisFaceNormals[i] + thisNormal;
            thisFaceNormals[i+1] = thisFaceNormals[i+1] + thisNormal;
            thisFaceNormals[i+2] = thisFaceNormals[i+2] + thisNormal;
        }

        //Now add each of these face normals to the actual stack of normals
        for (int i = 0; i < thisFaceNormals.size(); i++) {
            (*normals)[normalIndex[i]] = (*normals)[normalIndex[i]] + thisFaceNormals[i];
        }
    }
}

//Draw the face to the screen
bool Face::drawFace() {
    if (isFan) {
        glBegin(GL_TRIANGLE_FAN);
    } else {
        glBegin(GL_TRIANGLE_STRIP);
    }
    if (normals != NULL && textures != NULL && vertices != NULL) {
        for (int i = 0; i < vertexIndex.size(); i++) {
            (*normals)[normalIndex[i]].doNormal();
            (*textures)[textureIndex[i]].doTex();
            (*vertices)[vertexIndex[i]].doVertex();
        }
    } else if (normals != NULL && vertices != NULL) {
        for (int i = 0; i < vertexIndex.size(); i++) {
            (*normals)[normalIndex[i]].doNormal();
            (*vertices)[vertexIndex[i]].doVertex();
        }
    } else if (textures != NULL && vertices != NULL) {
        for (int i = 0; i < vertexIndex.size(); i++) {
            (*textures)[textureIndex[i]].doTex();
            (*vertices)[vertexIndex[i]].doVertex();
        }
    } else if (vertices != NULL) {
        for (int i = 0; i < vertexIndex.size(); i++) {
            (*vertices)[vertexIndex[i]].doVertex();
        }
    }
    glEnd();
    return true;
}
