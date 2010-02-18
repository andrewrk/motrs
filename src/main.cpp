#include "MainWindow.h"
#include "Config.h"

int main(int argc, char* argv[]) {
    Config::initialize(argc, argv, "config.ini");
    return MainWindow().exec();
}
