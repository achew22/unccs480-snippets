/**
 * This is the sample app from Code::blocks
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is Sample harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 *
 * Extended and set to the GLUTLoader
 * singleton by Andrew Allen October 2, 2008
 */

#include <GL/glut.h>
#include "SampleLoader.h"

int main(int argc, char *argv[])
{
    SampleLoader::start(argc, argv);
    return 0;
}
