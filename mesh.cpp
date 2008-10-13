#include <iostream>
#include <sstream>

#include "mesh.h"

Mesh::Mesh()
{
}

void Mesh::loadObj(std::string filename)
{
    glFrontFace(GL_CW);
    std::fstream instream;
    instream.open(filename.c_str());
    //printf("Mesh::Loading mesh from %s\n", filename.c_str());
    if (!instream.is_open())
    {
        //printf("Mesh::Error openin file");
        return;
    }
    while (!instream.eof())
    {
        std::string currentLine;
        getline(instream, currentLine);
        //printf("Mesh::Currentline is %s\n", currentLine.c_str());
        if (currentLine != "")  //Make sure that there is a first character to avoid errors
        {
            int temp;   //Used for throwing away dumps in sscanf
            if (sscanf(currentLine.c_str(), "v %f%*s", &temp) == 1) {   //This is a vertex
                vertices.push_back(Point3(currentLine));
            } else if (sscanf(currentLine.c_str(), "vt %f%*s", &temp) == 1) {   //This is a texture vertex
                //This is where I would do something with the texture guys
            } else if (sscanf(currentLine.c_str(), "vn %f%*s", &temp) == 1) {   //This is a normal
                printf("Loading a normal\n");
                normals.push_back(Point3(currentLine));
            } else if (sscanf(currentLine.c_str(), "f %*s") == 0) {  //This is a face
                if (sscanf(currentLine.c_str(), "f %i/%i/%i%*s", &temp, &temp, &temp) == 3) {   //Vertex, textures, and normals
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
                            //Throw an error here
                        } else {
                            vertIndex -= 1; //Subtract one, since .obj files begin indexing at 1 instead of 0
                            textIndex -= 1;
                            normIndex -= 1;
                            if (vertIndex < 0 || vertIndex >= vertices.size()) {
                                //Throw an error here
                            } else {
                                faceVerts.push_back(vertIndex);
                            }
                            if (textIndex < 0 || textIndex >= textures.size()) {
                                //Throw an error here
                            } else {
                                faceTexts.push_back(textIndex);
                            }
                            if (normIndex < 0 || normIndex >= normals.size()) {
                                //Throw an error here
                            } else {
                                faceNorms.push_back(normIndex);
                            }
                        }
                    }
                    addFace(faceVerts, faceNorms);
                } else if (sscanf(currentLine.c_str(), "f %i//%i%*s", &temp, &temp) == 2) { //Vertex, normals
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
                            //Throw an error here
                        } else {
                            vertIndex -= 1; //Subtract one, since .obj files begin indexing at 1 instead of 0
                            normIndex -= 1;
                            if (vertIndex < 0 || vertIndex >= vertices.size()) {
                                //Throw and error here
                            } else {
                                faceVerts.push_back(vertIndex);
                            }
                            if (normIndex < 0 || normIndex >= normals.size()) {
                                //Throw an error here
                            } else {
                                faceNorms.push_back(normIndex);
                            }
                        }
                    }
                    addFace(faceVerts, faceNorms);
                } else if (sscanf(currentLine.c_str(), "f %i/%i%*s", &temp, &temp) == 2) {  //Vertex, textures
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
                            //Throw an error here
                        } else {
                            vertIndex -= 1; //Subtract one, since .obj files begin indexing at 1 instead of 0
                            textIndex -= 1;
                            if (vertIndex < 0 || vertIndex >= vertices.size()) {
                                //Throw and error here
                            } else {
                                faceVerts.push_back(vertIndex);
                            }
                            if (textIndex < 0 || textIndex >= textures.size()) {
                                //Throw an error here
                            } else {
                                faceTexts.push_back(textIndex);
                            }
                        }
                    }
                    addFace(faceVerts);
                } else if (sscanf(currentLine.c_str(), "f %i%*s", &temp) == 1) { //Just vertex
                    //printf("Mesh::Pushing back a face\n");
                    std::vector<int> faceVerts;
                    std::string facePiece = "";
                    std::stringstream curLineStream(currentLine);
                    getline(curLineStream, facePiece, ' '); //Get the piece that is just 'f'
                    while(!curLineStream.eof())
                    {
                        getline(curLineStream, facePiece, ' ');
                        //printf("Mesh::facePiece is %s, ", facePiece.c_str());
                        //For now, just get the vertex, ignore the other stuff
                        int vertIndex;
                        if (sscanf(facePiece.c_str(), "%i%*s", &vertIndex) == 1) {    //If it finds it
                            vertIndex -= 1;     //For whatever reason, in .obj files, each list starts with index 1
                            //printf("vertIndex is %i\n", vertIndex);
                            if (vertIndex >= 0 && vertIndex < vertices.size()) {      //If the index is in a valid range
                                faceVerts.push_back(vertIndex);
                            } else {
                                printf("Error: vertIndex in MeshObjLoader was out of range\n");
                            }
                        } else {
                            printf("Error: MeshObjLoader: %s is not the proper format for a face piece\n", facePiece.c_str());
                        }
                    }
                    addFace(faceVerts);
                }
            } else {
                printf("Got something in the Obj file that wasn't what I expected: %s\n", currentLine.c_str());
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

void Mesh::addFace(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes) {
    Face newFace;
    newFace.registerVertices(&(this->vertices));
    newFace.registerVertexIndexes(vertIndexes);
    newFace.registerNormals(&(this->vertices));
    newFace.registerNormalIndexes(normalIndexes);
    faces.push_back(newFace);
    printf("Created a face: ");
    for (int i = 0; i < vertIndexes.size(); i++) {
        printf("%i/%i ", vertIndexes[i], normalIndexes[i]);
    }
    printf("\n");
}

void Mesh::addFace(std::vector<int>& vertIndexes) {
    Face newFace;
    newFace.registerVertices(&(this->vertices));
    newFace.registerVertexIndexes(vertIndexes);
    faces.push_back(newFace);
}

//Adds two faces, facing in both directions, so that one can be sure that neither side is see through
void Mesh::addFaceAndReverse(std::vector<int>& vertIndexes, std::vector<int>& normalIndexes) {
    addFace(vertIndexes, normalIndexes);
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
    addFace(newVerts, newNorms);
}

void Mesh::addFaceAndReverse(std::vector<int>& vertIndexes)
{
    addFace(vertIndexes);
    std::vector<int> newVerts;
    newVerts.resize(vertIndexes.size());
    for (int i = 0; i < vertIndexes.size(); i++)
    {
        newVerts[i] = vertIndexes[vertIndexes.size() - i - 1];
    }
    addFace(newVerts);
}

bool Mesh::drawMesh() {
    for (int i = 0; i < faces.size(); i++) {
        if (!faces[i].drawFace()) {
            return false;
        }
    }
    return true;
}
