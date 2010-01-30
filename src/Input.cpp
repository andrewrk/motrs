#include "Input.h"

#include "Utils.h"

bool Input::s_state[] = {false};
bool Input::s_lastState[] = {false};
SDLKey Input::s_map[] = {SDLK_UNKNOWN};


bool Input::state(Key key) {
    return s_state[key];
}

bool Input::justPressed(Key key) {
    return s_state[key] && !s_lastState[key];
}

bool Input::init() {
    std::map<std::string, std::string> props;
    if (!Utils::loadProperties("input.config", props))
        return false;

    return true;
}

void Input::refresh() {
    SDL_PumpEvents();

    // tmp:
    s_map[North] = SDLK_w;
    s_map[East] = SDLK_d;
    s_map[South] = SDLK_s;
    s_map[West] = SDLK_a;
    s_map[Jump] = SDLK_SPACE;
    s_map[Attack_1] = SDLK_j;

    Uint8 * keystate = SDL_GetKeyState(NULL);
    for (int k = 0; k < Key_size; k++) {
        s_lastState[k] = s_state[k];
        s_state[k] = keystate[s_map[k]];
    }
}
