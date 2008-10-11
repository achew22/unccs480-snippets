#include <string>

#include "GUIText.h"


GUIText::GUIText(int setX, int setY, int setWidth, int setHeight, Point3 setColor) {
    x = setX;
    y = setY;
    width = setWidth;
    height = setHeight;
    color = setColor;
}

//Overloaded virtual function to draw
void GUIText::draw() {

}

//Set the text to display
void GUIText::setText(std::string toBe) {
    this->textToShow = toBe;
}

