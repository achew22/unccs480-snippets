#include <iostream>

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

void Mesh::addFace(std::vector<Point3>& points)
{
    Face newFace;
    for (int i = 0; i < points.size(); i++)
    {
        newFace.addPoint(points[i]);
    }
    faces.push_back(newFace);
}

void Mesh::drawMesh()
{
    for (int i = 0; i < faces.size(); i++)
    {
        faces[i].drawFace();
    }
}
