#include "app.h"
int main(int argc, char *argv[]) {
    printf("This is a test\n");
    App *myApp = new App();
    return myApp->loop();
}
