#ifndef _INPUT_H_
#define _INPUT_H_

#include "SDL.h"

#include <map>
#include <string>


// No mouse support yet. Also, if you want to type your name, too bad.
class Input {
public:
    enum Key {
        // Movement
        North, East, South, West,
        Jump,

        // Fighting
        Attack_1,

        Key_size
    };

    static void init();

    // call this at the start of each frame.
    static void refresh();

    // whether a key is depressed currently.
    static bool state(Key key);
    // whether a key has just been pressed (like a key-down event).
    static bool justPressed(Key key);

private:
    static bool s_state[Key_size];
    static bool s_lastState[Key_size];
    static SDLKey s_map[Key_size];

    static std::map<std::string, Key> s_namesToKeys;
};

#endif
