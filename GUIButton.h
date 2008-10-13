#include <string>

#include <GL/gl.h>

#include "point.h"
#include "GUIElement.h"

class GUIButton : public GUIElement {
private:
    int* enterCallback;
    int* exitCallback;

    Point3 color;
    //Picture to map onto
    std::string text;
    Point3 textColor;

    int x,y;
    int height,width;

    bool (*callback)(void);

public:

    void draw();
    bool key(SDL_Event event);
    bool mouse(SDL_Event event);
    GUIButton();
    GUIButton(int setX, int setY, int setWidth, int setHeight, Point3 setTextColor, Point3 setColor, std::string setText, bool (*setCallback)());
    //GUIButton(Point3 stateColor, std::string stateText, Point3 stateTextColor);

    //void SetEnterCallback();
    //void SetEndCallback();
};
