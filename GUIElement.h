#ifndef GUIELEMENT_H_INCLUDED
#define GUIELEMENT_H_INCLUDED

#include <SDL/SDL.h>
class GUIElement {
public:
    virtual void draw() =0;
    virtual bool key(SDL_Event event) =0;
    virtual bool mouse(SDL_Event event) =0;
};

#endif // GUIELEMENT_H_INCLUDED
