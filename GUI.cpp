#include "GUI.h"

GUI::GUI() {

}

bool GUI::draw() {
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->draw();
    }
}

int GUI::addElement(GUIElement * toAdd){
    elements.push_back(toAdd);
}
