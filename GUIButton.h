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
public:

    void draw();
    bool key(SDL_Event event);
    bool mouse(SDL_Event event);
    GUIButton();
    //GUIButton(Point3 stateColor, std::string stateText, Point3 stateTextColor);

    //void SetEnterCallback();
    //void SetEndCallback();
};
