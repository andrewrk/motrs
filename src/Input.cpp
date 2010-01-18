#include "Input.h"

bool Input::m_state[] = {false};
bool Input::m_lastState[] = {false};
SDLKey Input::m_map[] = {SDLK_UNKNOWN};


bool Input::state(Key key) {
    return m_state[key];
}

bool Input::justPressed(Key key) {
    return m_state[key] && !m_lastState[key];
}

void Input::refresh() {
    SDL_PumpEvents();

    // tmp:
    m_map[North] = SDLK_w;
    m_map[East] = SDLK_d;
    m_map[South] = SDLK_s;
    m_map[West] = SDLK_a;

    Uint8 * keystate = SDL_GetKeyState(NULL);
    for (int k = 0; k < Key_size; k++) {
        m_lastState[k] = m_state[k];
        m_state[k] = keystate[m_map[k]];
    }
}
