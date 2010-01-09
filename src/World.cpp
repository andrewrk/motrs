#include "World.h"

World::World(const char * buffer)
{
    int* count = (int*)buffer;
//    WorldMap * cursor = buffer + sizeof(int);
    m_storyCount = 0;
    for (int i = 0; i < *count; i++) {
        // TODO: andy fill this in
//        m_maps.push_back(*cursor++);
//        m_storyCount = MAX(m_storyCount,
    }
}
