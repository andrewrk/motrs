#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>

class Input {
public:
    enum InputKey {
        // movement
        Up, Right, Down, Left,
        // Actions
        Attack1, Attack2, Jump, ToggleRun,
    };

    static bool keyState(InputKey key);

    static const std::string description(InputKey key);
};

#endif
