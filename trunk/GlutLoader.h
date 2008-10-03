#ifndef GLUTLOADER_H_INCLUDED
#define GLUTLOADER_H_INCLUDED

class GLUTLoader {
public:
    static void CallBackDisplayFunc(void);
    static void CallBackIdleFunc(void);
    static void CallBackKeyboardFunc(unsigned char key, int x, int y);
    static void CallBackMotionFunc(int x, int y);
    static void CallBackMouseFunc(int button, int state, int x, int y);
    static void CallBackPassiveMotionFunc(int x, int y);
    static void CallBackReshapeFunc(int w, int h);
    static void CallBackSpecialFunc(int key, int x, int y);
    static void CallBackVisibilityFunc(int visible);

    //Track the idle function being enabled
    static int idleFunctionEnabled;

    static void start();

    void  DisableIdleFunction(void);
    void  EnableIdleFunction(void);
    int   IdleFunctionEnabled(void);

};

#endif // GLUTLOADER_H_INCLUDED
