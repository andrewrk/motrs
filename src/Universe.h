#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include "World.h"
#include <vector>

/* Universe - a list of all worlds.
 *
 */

class Universe
{
    public:
        Universe(const char * buffer);
        ~Universe();

        // did the universe load ok?
        bool isGood();

    private:
        std::vector<World *> m_worlds;
};

#endif // _UNIVERSE_H_
