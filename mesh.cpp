#include <iostream>
#include <sstream>

#include "mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::string filename)
{
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
        size_t lastPos = -1;
        size_t pos = currentLine.find_first_of(':');    //Individual points are seperated by ':'s
        std::vector<Point3> points;
        do
        {
            //printf("Mesh::Adding a point:\n");
            points.push_back(Point3(currentLine.substr(lastPos + 1, pos)));
            //printf("Mesh::Point added is %f, %f, %f\n", points[points.size() - 1].x, points[points.size() - 1].y, points[points.size() - 1].z);
            lastPos = pos;
            pos = currentLine.find_first_of(':',lastPos + 1);
            //printf("Mesh::Finished adding said point\n");
        } while (lastPos != std::string::npos);
        addFace(points);
    }
    instream.close();
}

void Mesh::loadObj(std::string filename)
{
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
            if (sscanf(currentLine.c_str(), "v %f%*s", &temp) == 1) {
                //printf("Mesh::Pushing back a vector\n");
                vertices.push_back(Point3(currentLine));
            } else if (sscanf(currentLine.c_str(), "f %f%*s", &temp) == 1) {
                //printf("Mesh::Pushing back a face\n");
                std::vector<Point3> facePoints;
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
                            facePoints.push_back(vertices[vertIndex]);
                        } else {
                            printf("Error: vertIndex in MeshObjLoader was out of range\n");
                        }
                    } else {
                        printf("Error: MeshObjLoader: %s is not the proper format for a face piece\n", facePiece.c_str());
                    }
                }
                addFaceAndReverse(facePoints);
            } else if (sscanf(currentLine.c_str(), "vt %f%*s", &temp) == 1) {
                //This is where I would do something with the texture guys
            } else if (sscanf(currentLine.c_str(), "v %f%*s", &temp) == 1) {
                //This is where I would do something with the normal guys
            } else {
                printf("Got something in the Obj file that wasn't what I expected: %s\n", currentLine.c_str());
            }
        }
    }

    /* For printing out a mesh after loading * /
    printf("Loaded a mesh, vertices are:\n");
    for (int i = 0; i < vertices.size(); i++)
    {
        printf("   %i: <%f, %f, %f>\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
    }
    /**/

    instream.close();
}

void Mesh::addFace(std::vector<Point3>& points)
{
    Face newFace;
    //printf("Adding a face: vertices to follow:\n");
    for (int i = 0; i < points.size(); i++)
    {
        //printf("    <%f, %f, %f>\n", points[i].x, points[i].y, points[i].z);
        newFace.addPoint(points[i]);
    }
    faces.push_back(newFace);
}

//Adds two faces, facing in both directions, so that one can be sure that neither side is see through
void Mesh::addFaceAndReverse(std::vector<Point3>& points)
{
    addFace(points);
    std::vector<Point3> newPoints;
    newPoints.resize(points.size());
    for (int i = 0; i < points.size(); i++)
    {
        newPoints[i] = points[points.size() - i - 1];
    }
    addFace(newPoints);
}

bool Mesh::drawMesh() {
    for (int i = 0; i < faces.size(); i++) {
        if (!faces[i].drawFace()) {
            return false;
        }
    }
    return true;
}
