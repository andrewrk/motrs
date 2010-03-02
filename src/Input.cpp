#include "Input.h"

#include "Utils.h"
#include "Config.h"

Input::Input(const sf::Input & input) :
    m_input(input)
{
    // clear to 0
    for (int k=0; k<InputActionCount; ++k) {
        m_state[k] = false;
        m_lastState[k] = false;
        m_map[k] = (Input::KeyCode) -1;
    }

    // read settings
    Config * conf = Config::instance();
    m_map[North] = conf->keyNorth();
    m_map[East] = conf->keyEast();
    m_map[South] = conf->keySouth();
    m_map[West] = conf->keyWest();
    m_map[Jump] = conf->keyJump();
    m_map[Attack_1] = conf->keyAttack1();
}

bool Input::state(Action key)
{
    return m_state[key];
}

bool Input::justPressed(Action key)
{
    return m_state[key] && !m_lastState[key];
}

void Input::refresh()
{
    for (int k = 0; k < InputActionCount; ++k) {
        m_lastState[k] = m_state[k];
        m_state[k] = m_input.IsKeyDown((sf::Key::Code)m_map[k]);
    }
}
