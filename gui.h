#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

class GUI {
private:
    //std::vector<GUIElement> elements;
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
    //int addElement(GUIElement * toAdd);
    bool removeElemnt(int indexToRemove);

    //Return true to keep dispatching
    bool click(int x,int y,int button);
    bool key (int charCode);

    bool draw();

    /******
    Put in the getters and setters
    ******/

};

#endif // GUI_H_INCLUDED
