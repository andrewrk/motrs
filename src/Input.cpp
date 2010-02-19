#include "Input.h"

#include "Utils.h"
#include "Config.h"

bool Input::s_state[] = {false};
bool Input::s_lastState[] = {false};
SDLKey Input::s_map[] = {SDLK_UNKNOWN};
std::map<std::string, Input::Key> Input::s_namesToKeys;


bool Input::state(Key key) {
    return s_state[key];
}

bool Input::justPressed(Key key) {
    return s_state[key] && !s_lastState[key];
}

void Input::init() {
    // read settings
    Config * conf = Config::instance();
    s_map[North] = conf->keyNorth();
    s_map[East] = conf->keyEast();
    s_map[South] = conf->keySouth();
    s_map[West] = conf->keyWest();
    s_map[Jump] = conf->keyJump();
    s_map[Attack_1] = conf->keyAttack1();
}

void Input::refresh() {
    SDL_PumpEvents();

    Uint8 * keystate = SDL_GetKeyState(NULL);
    for (int k = 0; k < Key_size; k++) {
        s_lastState[k] = s_state[k];
        s_state[k] = keystate[s_map[k]];
    }
}
