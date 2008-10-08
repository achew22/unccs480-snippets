#include <string>

#include <Gl/gl.h>

#include "button.h"

ButtonState::ButtonState()
{

}
ButtonState(Point3 stateColor, std::string stateText, Point3 stateTextColor);
void SetEnterCallback();
void SetEndCallback();
