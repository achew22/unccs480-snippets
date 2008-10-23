#ifndef GUITEXTHANDLER_H_INCLUDED
#define GUITEXTHANDLER_H_INCLUDED

#include "GUIElement.h"

class GUITextHandler : public GUIElement {
public:
    GUITextHandler(bool (*setCallback)(SDL_Event));

    bool (*keyCallback)(SDL_Event event);

    bool key(SDL_Event event);
    bool mouse(SDL_Event event);

    void draw();
};

#endif // GUITEXTHANDLER_H_INCLUDED
