#include "Input.h"

#include "Debug.h"
#include "Utils.h"

bool Input::m_boolState[] = {false};
SDLKey Input::m_boolMap[] = {SDLK_UNKNOWN};


bool Input::boolState(Key key) {
    return m_boolState[key];
}

const std::string Input::description(Key key) {
    switch (key) {
        case Up: return "Up";
        case Right: return "Right";
        case Down: return "Down";
        case Left: return "Left";
        case Attack1: return "Attack 1";
        case Attack2: return "Attack 2";
        case Jump: return "Jump/Dive";
        case ToggleRun: return "Toggle Run";
        default: Debug::assert(false, Utils::intToString(key));
    }
}

void Input::refresh() {
    m_boolMap[Up] = SDLK_w;
    m_boolMap[Right] = SDLK_d;
    m_boolMap[Down] = SDLK_s;
    m_boolMap[Left] = SDLK_a;
    Uint8 * keystate = SDL_GetKeyState(NULL);
    for (int k = 0; k < Key_size; k++)
        m_boolState[k] = keystate[m_boolMap[k]];
    // if SDL_GetKeyState doesn't work without SDL_PumpEvents, use this for debugging:
    // m_boolState[Right] = true;
}
