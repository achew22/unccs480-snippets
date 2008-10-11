#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <vector>

#include "GUIElement.h"

class GUI {
private:
    std::vector<GUIElement *> elements;
    int xOffset;
    int yOffset;
    int width;
    int height;
    int xMax;
    int yMax;
public:
    GUI(int setXOffset, int setYOffset, int setWidth, int setHeight, int setXMax, int setYMax);
    GUI();

    //Return vertex index of the new element
    int addElement(GUIElement * toAdd);
    bool removeElement(int indexToRemove);

    //Return true to keep dispatching
    bool click(int x,int y,int button);
    bool key (SDL_Event event);
    bool mouse(SDL_Event event);

    bool draw();

    static GUI * pinstance;

    /******
    Put in the getters and setters
    ******/

};

#endif // GUI_H_INCLUDED
