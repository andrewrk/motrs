#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>

#include "SDL.h"

class Input {
public:
    enum Key {
        // movement
        Up, Right, Down, Left,
        // Actions
        Attack1, Attack2, Jump, ToggleRun,

        Key_size
    };

    static bool boolState(Key key);

    static void refresh();

    static const std::string description(Key key);
private:
    static bool m_boolState[Key_size];
    static SDLKey m_boolMap[Key_size];
};

#endif
