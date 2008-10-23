#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "cameraPosition.h"
#include "point.h"

class Character {
public:
    double x,y,z; ///Position
    double rotX, rotY, rotZ; ///Rotational components
    Point3 lookingDirection; ///Direction we are looking

public:

    void setPosition(Point3 & eyePosition, Point3 & lookAtPosition); ///<Overloadable function for the camera to get the position of the object



    //Overloaded functions to make this work
    //I decided to do these as overloaded instead of just
    //assuming there was one "mesh" because I might want to
    //make an app where a gaggle (that is the standard metric
    //unit) of sheep follow a herdsman or something like that
    //think olimar or something like that
    virtual void draw() =0; ///<Overloaded draw function
};

#endif // CHARACTER_H_INCLUDED
