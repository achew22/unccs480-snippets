#include <iostream>
#include <sstream>

#include "mesh.h"
//#include "textureManager.h"
#include "error.h"

Mesh::Mesh()
{
}

void Mesh::loadObj(std::string filename)
{
    std::fstream instream;
    instream.open(filename.c_str());
    Error::debug("Mesh::Loading mesh from %s\n", filename.c_str());
    if (!instream.is_open())
    {
        Error::error("Mesh::Error opening file: filename = %s\n", filename.c_str());
        return;
    }
    while (!instream.eof())
    {
        std::string currentLine;
        getline(instream, currentLine);
        //Error::debug("Mesh::Currentline is %s\n", currentLine.c_str());
        if (currentLine != "")  //Make sure that there is a first character to avoid errors
        {
            int temp;   //Used for throwing away dumps in sscanf
            if (sscanf(currentLine.c_str(), "v %f%*s", &temp) == 1) {   //This is a vertex
                vertices.push_back(Point3(currentLine));
            } else if (sscanf(currentLine.c_str(), "vt %f%*s", &temp) == 1) {   //This is a texture vertex
                texturePts.push_back(Point2(currentLine));
            } else if (sscanf(currentLine.c_str(), "vn %f%*s", &temp) == 1) {   //This is a normal
                normals.push_back(Point3(currentLine));
            } else if (sscanf(currentLine.c_str(), "f %*s") == 0) {  //This is a face
                if (sscanf(currentLine.c_str(), "f %i/%i/%i%*s", &temp, &temp, &temp) == 3) {   //Vertex, textures, and normals
                    //Error::debug("Mesh::Pushing back a face with vertex/texture/normal\n");
                    std::vector<int> faceVerts;  //
                    std::vector<int> faceTexts;
                    std::vector<int> faceNorms;
                    std::string facePiece = "";
                    std::stringstream curLineStream(currentLine);
                    getline(curLineStream, facePiece, ' '); //Get the piece that is just 'f'
                    while(!curLineStream.eof())
                    {
                        getline(curLineStream, facePiece, ' ');
                        int vertIndex, textIndex, normIndex;
                        if (sscanf(facePiece.c_str(), "%i/%i/%i", &vertIndex, &textIndex, &normIndex) != 3) {
                            Error::error("Mesh::Trying to create a face from unusual data: string = %s\n", facePiece.c_str());
                        } else {
                            vertIndex -= 1; //Subtract one, since .obj files begin indexing at 1 instead of 0
                            textIndex -= 1;
                            normIndex -= 1;
                            if (vertIndex < 0 || vertIndex >= vertices.size()) {
                                Error::error("Mesh::Tried to use a vertex index that was out of range: index = %i\n", vertIndex);
                            } else {
                                faceVerts.push_back(vertIndex);
                            }
                            if (textIndex < 0 || textIndex >= texturePts.size()) {
                                Error::error("Mesh::Tried to use a texture index that was out of range: index = %i\n", textIndex);
                            } else {
                                faceTexts.push_back(textIndex);
                            }
                            if (normIndex < 0 || normIndex >= normals.size()) {
                                Error::error("Mesh::Tried to use a normal index that was out of range: index = %i\n", normIndex);
                            } else {
                                faceNorms.push_back(normIndex);
                            }
                        }
                    }
                    addFaceVTN(faceVerts, faceTexts, faceNorms);
                } else if (sscanf(currentLine.c_str(), "f %i//%i%*s", &temp, &temp) == 2) { //Vertex, normals
                    //Error::debug("Mesh::Pushing back a face with just a vertex//normal\n");
                    std::vector<int> faceVerts;  //
                    std::vector<int> faceNorms;
                    std::string facePiece = "";
                    std::stringstream curLineStream(currentLine);
                    getline(curLineStream, facePiece, ' '); //Get the piece that is just 'f'
                    while(!curLineStream.eof())
                    {
                        getline(curLineStream, facePiece, ' ');
                        int vertIndex, normIndex;
                        if (sscanf(facePiece.c_str(), "%i//%i", &vertIndex, &normIndex) != 2) {
                            Error::error("Mesh::Trying to create a face from unusual data: string = %s\n", facePiece.c_str());
                        } else {
                            vertIndex -= 1; //Subtract one, since .obj files begin indexing at 1 instead of 0
                            normIndex -= 1;
                            if (vertIndex < 0 || vertIndex >= vertices.size()) {
                                Error::error("Mesh::Tried to use a vertex index that was out of range: index = %i\n", vertIndex);
                            } else {
                                faceVerts.push_back(vertIndex);
                            }
                            if (normIndex < 0 || normIndex >= normals.size()) {
                                Error::error("Mesh::Tried to use a normal index that was out of range: index = %i\n", normIndex);
                            } else {
                                faceNorms.push_back(normIndex);
                            }
                        }
                    }
                    addFaceVN(faceVerts, faceNorms);
                } else if (sscanf(currentLine.c_str(), "f %i/%i%*s", &temp, &temp) == 2) {  //Vertex, textures
                    //Error::debug("Mesh::Pushing back a face with just a vertex/texture\n");
                    std::vector<int> faceVerts;  //
                    std::vector<int> faceTexts;
                    std::string facePiece = "";
                    std::stringstream curLineStream(currentLine);
                    getline(curLineStream, facePiece, ' '); //Get the piece that is just 'f'
                    while(!curLineStream.eof())
                    {
                        getline(curLineStream, facePiece, ' ');
                        int vertIndex, textIndex;
                        if (sscanf(facePiece.c_str(), "%i/%i", &vertIndex, &textIndex) != 2) {
                            Error::error("Mesh::Trying to create a face from unusual data: string = %s\n", facePiece.c_str());
                        } else {
                            vertIndex -= 1; //Subtract one, since .obj files begin indexing at 1 instead of 0
                            textIndex -= 1;
                            if (vertIndex < 0 || vertIndex >= vertices.size()) {
                                Error::error("Mesh::Tried to use a vertex index that was out of range: index = %i\n", vertIndex);
                            } else {
                                faceVerts.push_back(vertIndex);
                            }
                            if (textIndex < 0 || textIndex >= texturePts.size()) {
                                Error::error("Mesh::Tried to use a texture index that was out of range: index = %i\n", textIndex);
                            } else {
                                faceTexts.push_back(textIndex);
                            }
                        }
                    }
                    addFaceVT(faceVerts, faceTexts);
                } else if (sscanf(currentLine.c_str(), "f %i%*s", &temp) == 1) { //Just vertex
                    //Error::debug("Mesh::Pushing back a face with just a vertex\n");
                    std::vector<int> faceVerts;
                    std::string facePiece = "";
                    std::stringstream curLineStream(currentLine);
                    getline(curLineStream, facePiece, ' '); //Get the piece that is just 'f'
                    while(!curLineStream.eof())
                    {
                        getline(curLineStream, facePiece, ' ');
                        int vertIndex;
                        if (sscanf(facePiece.c_str(), "%i%*s", &vertIndex) == 1) {    //If it finds it
                            vertIndex -= 1;     //For whatever reason, in .obj files, each list starts with index 1
                            if (vertIndex >= 0 && vertIndex < vertices.size()) {      //If the index is in a valid range
                                faceVerts.push_back(vertIndex);
                            } else {
                                Error::error("Mesh::Tried to use a vertex index that was out of range: index = %i\n", vertIndex);
                            }
                        } else {
                            Error::error("Mesh::Trying to create a face from unusual data: string = %s\n", facePiece.c_str());
                        }
                    }
                    addFaceV(faceVerts);
                }
            } else {
                //Error::debug("Mesh::Got something in the Obj file that wasn't what I expected: %s\n", currentLine.c_str());
            }
        }
    }

    /* For printing out a mesh after loading * /
    printf("Loaded a mesh, vertices are:\n");
    for (int i = 0; i < vertices.size(); i++) {
        printf("   %i: <%f, %f, %f>\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
    }
    printf("Loaded a mesh, textures are:\n");
    for (int i = 0; i < textures.size(); i++) {
        printf("   %i: <%f, %f, %f>\n", i, textures[i].x, textures[i].y, textures[i].z);
    }
    printf("Loaded a mesh, normals are:\n");
    for (int i = 0; i < normals.size(); i++) {
        printf("   %i: <%f, %f, %f>\n", i, normals[i].x, normals[i].y, normals[i].z);
    }
    /**/

    instream.close();
}

void Mesh::loadTexture(std::string filename) {
    //textureId = TextureManager::getInstance()->loadBMP(filename);
}

void Mesh::addFaceVTN(std::vector<int>& vertIndexes, std::vector<int>& textIndexes, std::vector<int>& normalIndexes) {
    Face newFace;
    newFace.registerVertices(&(this->vertices));
    newFace.registerVertexIndexes(vertIndexes);
    newFace.registerTextures(&(this->texturePts));
    newFace.registerTextureIndexes(textIndexes);
    newFace.registerNormals(&(this->normals));
    newFace.registerNormalIndexes(normalIndexes);
    if (vertIndexes.size() != textIndexes.size() || vertIndexes.size() != normalIndexes.size() || textIndexes.size() != normalIndexes.size()) {
        Error::error("Mesh::addFaceVTN() There are %i vertices, %i texturePts, and %i normals\n", vertIndexes.size(), textIndexes.size(), normalIndexes.size());
    }
    faces.push_back(newFace);
}

void Mesh::addFaceVN(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes) {
    Face newFace;
    newFace.registerVertices(&(this->vertices));
    newFace.registerVertexIndexes(vertIndexes);
    newFace.registerNormals(&(this->normals));
    newFace.registerNormalIndexes(normalIndexes);
    faces.push_back(newFace);
    /*
    printf("Created a face: ");
    for (int i = 0; i < vertIndexes.size(); i++) {
        printf("%i/%i ", vertIndexes[i], normalIndexes[i]);
    }
    printf("\n");
    */
}

void Mesh::addFaceVT(std::vector<int>& vertIndexes, std::vector<int>& textIndexes) {
    Face newFace;
    newFace.registerVertices(&(this->vertices));
    newFace.registerVertexIndexes(vertIndexes);
    newFace.registerTextures(&(this->texturePts));
    newFace.registerTextureIndexes(textIndexes);

    //Register the normals anyway, even though they aren't being used
    newFace.registerNormals(&(this->normals));
    faces.push_back(newFace);
}

void Mesh::addFaceV(std::vector<int>& vertIndexes) {
    Face newFace;
    newFace.registerVertices(&(this->vertices));
    newFace.registerVertexIndexes(vertIndexes);

    //Register the normals anyway, even though they aren't being used
    newFace.registerNormals(&(this->normals));
    faces.push_back(newFace);
}

void Mesh::addFaceAndReverseVTN(std::vector<int>& vertIndexes, std::vector<int>& textIndexes, std::vector<int>& normalIndexes) {
    addFaceVTN(vertIndexes, textIndexes, normalIndexes);
    std::vector<int> newVerts;
    std::vector<int> newTexts;
    std::vector<int> newNorms;
    newVerts.resize(vertIndexes.size());
    newTexts.resize(textIndexes.size());
    newNorms.resize(normalIndexes.size());
    for (int i = 0; i < vertIndexes.size(); i++) {
        newVerts[i] = vertIndexes[vertIndexes.size() - i - 1];
    }
    for (int i = 0; i < textIndexes.size(); i++) {
        newTexts[i] = textIndexes[textIndexes.size() - i - 1];
    }
    for (int i = 0; i < normalIndexes.size(); i++) {
        newNorms[i] = normalIndexes[normalIndexes.size() - i - 1];
    }
    addFaceVTN(newVerts, newTexts, newNorms);
}

//Adds two faces, facing in both directions, so that one can be sure that neither side is see through
void Mesh::addFaceAndReverseVN(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes) {
    addFaceVN(vertIndexes, normalIndexes);
    std::vector<int> newVerts;
    std::vector<int> newNorms;
    newVerts.resize(vertIndexes.size());
    newNorms.resize(normalIndexes.size());
    for (int i = 0; i < vertIndexes.size(); i++) {
        newVerts[i] = vertIndexes[vertIndexes.size() - i - 1];
    }
    for (int i = 0; i < normalIndexes.size(); i++) {
        newNorms[i] = normalIndexes[normalIndexes.size() - i - 1];
    }
    addFaceVN(newVerts, newNorms);
}

//Adds two faces, facing in both directions, so that one can be sure that neither side is see through
void Mesh::addFaceAndReverseVT(std::vector<int>& vertIndexes, std::vector<int>& textIndexes) {
    addFaceVT(vertIndexes, textIndexes);
    std::vector<int> newVerts;
    std::vector<int> newTexts;
    newVerts.resize(vertIndexes.size());
    newTexts.resize(textIndexes.size());
    for (int i = 0; i < vertIndexes.size(); i++) {
        newVerts[i] = vertIndexes[vertIndexes.size() - i - 1];
    }
    for (int i = 0; i < textIndexes.size(); i++) {
        newTexts[i] = textIndexes[textIndexes.size() - i - 1];
    }
    addFaceVT(newVerts, newTexts);
}

void Mesh::addFaceAndReverseV(std::vector<int>& vertIndexes)
{
    addFaceV(vertIndexes);
    std::vector<int> newVerts;
    newVerts.resize(vertIndexes.size());
    for (int i = 0; i < vertIndexes.size(); i++)
    {
        newVerts[i] = vertIndexes[vertIndexes.size() - i - 1];
    }
    addFaceV(newVerts);
}

/**
 * This function will rewrite ALL normal vectors, causing total loss of normal data
 * To reobtain normal data, simply reload the .obj file
 */
void Mesh::calculateNormals() {
    //First, clear normal data
    normals.resize(vertices.size());
    for (int i = 0; i < normals.size(); i++) {
        normals[i] = Point3(0,0,0);
    }

    //This will add every face's normal to all of the vertices of that face
    //then reassign each normal index to the corresponding vertex index
    for (int i = 0; i < faces.size(); i++) {
        faces[i].addOwnNormals();
    }

    //This will normalize all normals, so that they are unit
    for (int i = 0; i < normals.size(); i++) {
        normals[i] = normals[i].getUnit();
    }
}

Point3 Mesh::calculateCenter() {
    Point3 center(0,0,0);
    for (int i = 0; i < vertices.size(); i++) {
        center = center + vertices[i]/((double)vertices.size());
    }
    return center;
}

double Mesh::calculateAverageRadius() {
    Point3 center = calculateCenter();
    double average = 0;
    for (int i = 0; i < vertices.size(); i++) {
        average = average + (vertices[i] - center).getMag()/((double)vertices.size());
    }
    return average;
}

void Mesh::resize(double newSize) {
    double curSize = calculateAverageRadius();
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = vertices[i] * newSize/curSize;
    }
}

bool Mesh::drawMesh() {
    glBindTexture(GL_TEXTURE_2D, textureId);
    for (int i = 0; i < faces.size(); i++) {
        if (!faces[i].drawFace()) {
            return false;
        }
    }
    return true;
}
