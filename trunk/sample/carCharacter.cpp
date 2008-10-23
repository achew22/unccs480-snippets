#include <string>

#include <SDL/SDL.h>

#include "error.h"
#include "carCharacter.h"
#include "cameraPosition.h"
#include "mesh.h"

/**
 * Default constructor to initialize all variables
 */
CarCharacter::CarCharacter() {
    //ŜmyMesh = new Mesh();

    //Set all variables to 0 the fast way
    rotX = rotY = rotX = x = y = z = 0;
    scaleFactor = 1;
}

/**
 * Load the requested texture file
 */
void CarCharacter::loadTexture(std::string pathToFile) {
    myMesh->loadTexture(pathToFile);
}

/**
 * Load the requested object file
 */
void CarCharacter::loadObj(std::string pathToFile) {
    myMesh->loadObj(pathToFile);
}

/**
 * Draw the current character (make sure to account for their rotation)
 */
void CarCharacter::draw() {
    glPushMatrix(); {
        glColor3d(1,1,1);

        //Make it big (or small)
        glScaled(scaleFactor, scaleFactor, scaleFactor);
/*
        glRotated(rotX, 1,0,0);
        glRotated(rotY, 0,1,0);
        glRotated(rotZ, 0,0,1);
*/

        glTranslated(x,y,z);

        myMesh->drawMesh();
    }
}

/**
 * Scale the current character
 */
void CarCharacter::scale(double scaleTo) {
    scaleFactor = scaleTo;
}


/**
 * Move right
 */
void CarCharacter::moveRight(bool toMove) {
    Error::log("This should do nothing!");
    x = 1;
    y = 1;
    z = 3;
}

/**
 * Move left
 */
void CarCharacter::moveLeft(bool toMove) {

}
