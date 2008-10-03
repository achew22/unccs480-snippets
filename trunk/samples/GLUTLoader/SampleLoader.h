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

#ifndef SAMPLELOADER_H_INCLUDED
#define SAMPLELOADER_H_INCLUDED
#include "../../unccs480-snippets/GlutLoader.h"
class SampleLoader : GLUTLoader {

private:
    static SampleLoader* pinstance;

protected:
    static void CallBackDisplayFunc(void);
    static void CallBackKeyboardFunc(unsigned char key, int x, int y);
    static void CallBackReshapeFunc(int width, int height);

    int slices;
    int stacks;

public:
    static SampleLoader* getInstance();

    static void start(int argc, char *argv[]);
//    SampleLoader();
};

#endif // SAMPLELOADER_H_INCLUDED
