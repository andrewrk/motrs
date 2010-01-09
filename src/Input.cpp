#include "Input.h"

bool Input::keyState(InputKey key) {
    return false;
}

const std::string Input::description(InputKey key) {
    switch (key) {
        case Up: return "Up";
        case Right: return "Right";
        case Down: return "Down";
        case Left: return "Left";
        case Attack1: return "Attack1";
        case Attack2: return "Attack2";
        case Jump: return "Jump/Dive";
        case ToggleRun: return "Toggle Run";
        default: throw 0;
    }
}




