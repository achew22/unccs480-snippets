#include "GUITextHandler.h"

GUITextHandler::GUITextHandler(bool (*setCallback)(SDL_Event)) {
    keyCallback = setCallback;
}

bool GUITextHandler::key(SDL_Event event) {
    return keyCallback(event);
}

bool GUITextHandler::mouse(SDL_Event event) {

}

void GUITextHandler::draw() {

}
