#ifndef GUITEXT_H_INCLUDED
#define GUITEXT_H_INCLUDED

#include <string>

#include "GUIElement.h"
#include "point.h"

class GUIText : public GUIElement {
private:
    std::string textToShow;

    int x,y,width,height;
    Point3 color;
public:
    GUIText(int setX, int setY, int setWidth, int setHeight, Point3 setColor);

    void draw();
    void setText(std::string toBe);
};

#endif // GUITEXT_H_INCLUDED
