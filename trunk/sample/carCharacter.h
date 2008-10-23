#ifndef CARCHARACTER_H_INCLUDED
#define CARCHARACTER_H_INCLUDED

#include <string>

#include "mesh.h"
#include "character.h"

class CarCharacter : public Character {
//class CarCharacter {
private:
    Mesh * myMesh;
    double scaleFactor;

public:
    CarCharacter();

    void scale(double scaleTo);

    void loadTexture(std::string pathToFile);
    void loadObj(std::string pathToFile);

    void draw();

    void moveRight(bool toMove);
    void moveLeft(bool toMove);
};

#endif // CARCHARACTER_H_INCLUDED
