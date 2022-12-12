#include "Application.h"

int main(int argc, char **argv) {
    Application App("BlockOut", "1.0");
    App.parseArguments(argc, argv);
    App.init();
    return App.run();
}