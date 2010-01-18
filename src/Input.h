#ifndef _INPUT_H_
#define _INPUT_H_

#include "SDL.h"


/** No mouse support yet. If you want to type your name, too bad. */
class Input {
public:
    enum Key {
        // Movement
        North, East, South, West,
        Jump,
        ActivateDash,
        ToggleRun,

        // Fighting
        Attack_1, Attack_2,
        Weapon_1, Weapon_2, Weapon_3, Weapon_4, Weapon_5,
        Weapon_6, Weapon_7, Weapon_8, Weapon_9, Weapon_0,

        // Misc
        Interact,
        TogglePause,

        Key_size
    };

    /** whether a key is depressed currently. */
    static bool state(Key key);
    /** whether a key has just been pressed (like a key-down event). */
    static bool justPressed(Key key);

    /** call this at the start of each frame. */
    static void refresh();

private:
    static bool m_state[Key_size];
    static bool m_lastState[Key_size];
    static SDLKey m_map[Key_size];
};

#endif
