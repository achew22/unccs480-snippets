#include "GlutLoader.h"
#include <stdlib.h>
#include <GL/glut.h>

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackDisplayFunc(void) {

}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackIdleFunc(void) {
    glutPostRedisplay();
}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackKeyboardFunc(unsigned char key, int x, int y) {
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

    }

    glutPostRedisplay();
}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackMotionFunc(int x, int y) {

}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackMouseFunc(int button, int state, int x, int y) {

}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackPassiveMotionFunc(int x, int y) {

}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackReshapeFunc(int w, int h) {

}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackSpecialFunc(int key, int x, int y) {

}

//Shell function to allow you to overload it in a higher class
void GLUTLoader::CallBackVisibilityFunc(int visible) {

}

//Start GLUT
void GLUTLoader::start() {
   glutDisplayFunc(CallBackDisplayFunc);
   glutIdleFunc(CallBackIdleFunc);
   glutKeyboardFunc(CallBackKeyboardFunc);
   glutSpecialFunc(CallBackSpecialFunc);
   glutMouseFunc(CallBackMouseFunc);
   glutMotionFunc(CallBackMotionFunc);
   glutPassiveMotionFunc(CallBackPassiveMotionFunc);
   glutReshapeFunc(CallBackReshapeFunc);
   glutVisibilityFunc(CallBackVisibilityFunc);

   glutMainLoop();
}
