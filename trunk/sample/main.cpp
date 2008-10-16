#include "app.h"
int main(int argc, char *argv[]) {
    return App::getInstance()->loop();
}
