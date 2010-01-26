#include "Entity.h"

#include "ResourceManager.h"
#include "Gameplay.h"

#include "Utils.h"

Entity::Entity(double radius, double centerOffsetX, double centerOffsetY) :
    m_centerX(0.0), m_centerY(0.0), m_radius(radius),
    m_velocityX(0.0), m_velocityY(0.0),
    m_layer(0),
    m_altitude(0.0), m_altitudeVelocity(0.0),
    m_direction(Center), m_movementMode(Stand),
    m_centerOffsetX(centerOffsetX), m_centerOffsetY(centerOffsetY)
{
    memset(m_standing, 0, sizeof(m_standing));
    memset(m_walking, 0, sizeof(m_walking));
    memset(m_running, 0, sizeof(m_running));
}

Entity * Entity::load(const char *buffer) {
    const char * cursor = buffer;

    int version = Utils::readInt(&cursor);
    if (version != 4) {
        std::cerr << "Unsupported Entity version: " << version << std::endl;
        return NULL;
    }

    double centerOffsetX = (double)Utils::readInt(&cursor);
    double centerOffsetY = (double)Utils::readInt(&cursor);
    double radius = (double)Utils::readInt(&cursor);

    Entity * entity = new Entity(radius, centerOffsetX, centerOffsetY);
    Graphic** movementGraphics[] = { entity->m_standing, entity->m_walking, entity->m_running };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            std::string graphicId = Utils::readString(&cursor);
            Graphic * graphic = ResourceManager::getGraphic(graphicId);
            if (graphic == NULL) {
                delete entity;
                return NULL;
            }
            movementGraphics[i][j] = graphic;
        }
    }
    return entity;
}

void Entity::draw(double screenX, double screenY) {
    Graphic** graphicList;
    switch (m_movementMode) {
    case Stand: graphicList = m_standing; break;
    case Walk: graphicList = m_walking; break;
    case Run: graphicList = m_running; break;
    case JumpUp: graphicList = m_running; break;
    case JumpDown: graphicList = m_standing; break;
    default: Debug::assert(false, "unrecognized movementMode.");
    }
    graphicList[m_direction]->draw(Gameplay::instance()->screen(),
                                   (int)(m_centerX - m_centerOffsetX - screenX),
                                   (int)(m_centerY - m_centerOffsetY - m_altitude - screenY));
}

