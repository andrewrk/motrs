#include "Input.h"

#include "Utils.h"

bool Input::s_state[] = {false};
bool Input::s_lastState[] = {false};
SDLKey Input::s_map[] = {SDLK_UNKNOWN};
std::map<std::string, Key> Input::s_namesToKeys;


bool Input::state(Key key) {
    return s_state[key];
}

bool Input::justPressed(Key key) {
    return s_state[key] && !s_lastState[key];
}

bool Input::init() {
    // defaults or whatever
    s_map[North] = SDLK_w;
    s_map[East] = SDLK_d;
    s_map[South] = SDLK_s;
    s_map[West] = SDLK_a;
    s_map[Jump] = SDLK_SPACE;
    s_map[Attack_1] = SDLK_j;

    // load names map
    {
        s_namesToKeys["North"] = North;
        s_namesToKeys["East"] = East;
        s_namesToKeys["South"] = South;
        s_namesToKeys["West"] = West;
        s_namesToKeys["Jump"] = Jump;
        s_namesToKeys["Attack_1"] = Attack_1;
    }

    std::map<std::string, std::string> props;
    if (!Utils::loadProperties("input.config", props))
        return false;

    for (std::map<std::string, std::string>::iterator iter = props.begin(); iter != props.end(); iter++) {
        std::string name = iter->first;
        std::string value = iter->second;
        std::map<std::string, Key>::iterator searchResult = s_namesToKeys.find(name);
        if (searchResult == s_namesToKeys.end()) {
            std::cerr << "Unknown key \"" << name << "\"." << std::endl;
            return false;
        }
        Key key = searchResult->second();
        SDLKey parsedValue = (SDLKey)Utils::stringToInt(value);
        s_map[key] = parsedValue;
    }

    return true;
}

void Input::refresh() {
    SDL_PumpEvents();

    Uint8 * keystate = SDL_GetKeyState(NULL);
    for (int k = 0; k < Key_size; k++) {
        s_lastState[k] = s_state[k];
        s_state[k] = keystate[s_map[k]];
    }
}
