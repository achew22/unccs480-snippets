#ifndef CAMERAPOSITION_H_INCLUDED
#define CAMERAPOSITION_H_INCLUDED

class CameraPosition {
public:
    double x,y,z, rotX, rotY, rotZ;

    CameraPosition(double setX,double setY,double setZ, double setRotX, double setRotY, double setRotZ) {
        x = setX;
        y = setY;
        z = setZ;
        rotX = setRotX;
        rotY = setRotY;
        rotZ = setRotZ;
    };
};

#endif // CAMERAPOSITION_H_INCLUDED
