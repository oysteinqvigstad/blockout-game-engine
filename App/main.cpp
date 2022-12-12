#include "LabApplication.h"

int main(int argc, char **argv) {
    LabApplication App("Lab 2", "1.0");
    App.parseArguments(argc, argv);
    App.init();
    return App.run();
}