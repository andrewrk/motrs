#include "Entity.h"

#include "ResourceManager.h"
#include "Gameplay.h"

#include "Utils.h"

Entity::Entity(const char * buffer) :
    m_good(true),
    m_currentGraphic(NULL),
    m_direction(Center), m_movementMode(Stand),
    m_centerOffsetX(0.0), m_centerOffsetY(0.0), m_radius(1.0),
    m_x(0.0), m_y(0.0), m_z(0), m_altitude(0.0)
{
    memset(m_standing, 0, sizeof(m_standing));
    memset(m_walking, 0, sizeof(m_walking));
    memset(m_running, 0, sizeof(m_running));

    const char * cursor = buffer;

    int version = Utils::readInt(&cursor);
    if (version != 3) {
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
            movement_graphics[i][j] = graphic;
        }
    }

    m_centerOffsetX = (double)Utils::readInt(&cursor);
    m_centerOffsetY = (double)Utils::readInt(&cursor);
    m_radius = (double)Utils::readInt(&cursor);
}

Entity::~Entity()
{
}

void Entity::setCenter(double x, double y) {
    m_x = x - m_centerOffsetX;
    m_y = y - m_centerOffsetY;
}

void Entity::setLayer(int z) {
    m_z = z;
}

void Entity::setOrientation(Direction direction) {
    m_direction = direction;
}

void Entity::setMovementMode(MovementMode movementMode) {
    m_movementMode = movementMode;
}

void Entity::draw(double screenX, double screenY) {
    Graphic** graphicList;
    switch (m_movementMode) {
        case Stand: graphicList = m_standing; break;
        case Walk: graphicList = m_walking; break;
        case Run: graphicList = m_running; break;
        default: Debug::assert(false, "unrecognized movementMode.");
    }
    graphicList[m_direction]->draw(Gameplay::instance()->screen(),(int)(m_x - screenX), (int)(m_y - screenY));
}

