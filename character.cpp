#include <math.h>

#include "character.h"
#include "cameraPosition.h"

/**
 * Return the position of the current character based on their x,y,z and rotational attributes
 */
void Character::setPosition(Point3 & eyePosition, Point3 & lookAtPosition) {


   eyePosition.x = x;
   eyePosition.y = y;
   eyePosition.z = z + 1;

   eyePosition.x = 7;
   printf("X: %f Y: %f Z: %f\n", x,y,z);

/*
   ///@todo Make this do the look at the right spot by default
   lookAtPosition->x = x + 100;
   lookAtPosition->y = y;
   lookAtPosition->z = z + 1;
*/

   lookAtPosition.x = 0;
   lookAtPosition.y = 0;
   lookAtPosition.z = 0;
}
