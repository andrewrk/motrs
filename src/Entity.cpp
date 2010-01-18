#include "Entity.h"

#include "Utils.h"

Entity::Entity(const char * buffer) :
    m_good(true),
    m_currentGraphic(NULL),
    m_directionPointing(Center),
    m_feetX(0.0), m_feetY(0.0), m_feetW(0.0), m_feetH(0.0),
    m_x(0.0), m_y(0.0), m_velX(0.0), m_velY(0.0)
{
    memset(m_standing, 0, sizeof(m_standing));
    memset(m_walking, 0, sizeof(m_walking));
    memset(m_running, 0, sizeof(m_running));

    const char * cursor = buffer;

    int version = Utils::readInt(&cursor);
    if (version != 1) {
        std::cerr << "Unsupport Entity version: " << version << std::endl;
        m_good = false;
        return;
    }

    Graphic** movement_graphics[] = {m_standing, m_walking, m_running};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            Graphic * graphic = ResourceManager::getGraphic(Utils::readString(&cursor));
            if (graphic == NULL) {
                m_good = false;
                return;
            }
            movement_graphics[j][i] = graphic;
        }
    }

    m_feetX = (double)Utils::readInt(&cursor);
    m_feetY = (double)Utils::readInt(&cursor);
    m_feetW = (double)Utils::readInt(&cursor);
    m_feetH = (double)Utils::readInt(&cursor);

}

Entity::~Entity()
{
}

