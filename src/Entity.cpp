#include "Entity.h"

#include "ResourceManager.h"
#include "Gameplay.h"

#include "Utils.h"

Entity::Entity(double radius, double centerOffsetX, double centerOffsetY, double speed, double mass) :
    m_centerX(0.0), m_centerY(0.0), m_radius(radius),
    m_velocityX(0.0), m_velocityY(0.0),
    m_layer(0),
    m_altitude(0.0), m_altitudeVelocity(0.0),
    m_speed(speed), m_mass(mass),
    m_direction(Center), m_movementMode(Stand),
    m_centerOffsetX(centerOffsetX), m_centerOffsetY(centerOffsetY),
    m_currentSequence(None), m_sequencePosition(0)
{
    memset(m_standing, 0, sizeof(m_standing));
    memset(m_walking, 0, sizeof(m_walking));
    memset(m_running, 0, sizeof(m_running));
    memset(m_sword, 0, sizeof(m_running));
}

Entity * Entity::load(const char *buffer) {
    const char * cursor = buffer;

    int version = Utils::readInt(&cursor);
    if (version != 6) {
        std::cerr << "Unsupported Entity version: " << version << std::endl;
        return NULL;
    }

    double centerOffsetX = (double)Utils::readInt(&cursor);
    double centerOffsetY = (double)Utils::readInt(&cursor);
    double radius = (double)Utils::readInt(&cursor);

    double speed = Utils::readDouble(&cursor);
    double mass = Utils::readDouble(&cursor);

    Entity * entity = new Entity(radius, centerOffsetX, centerOffsetY, speed, mass);
    Graphic** movementGraphics[] = { entity->m_standing, entity->m_walking, entity->m_running, entity->m_sword };
    for (int i = 0; i < 4; i++) {
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
    case Falling: graphicList = m_standing; break;
    default: Debug::assert(false, "unrecognized movementMode.");
    }
    // override with sequence graphics
    switch (m_currentSequence) {
    case None: break; // leave it alone
    case Sword: graphicList = m_sword; break;
    default: Debug::assert(false, "unrecognized Sequence.");
    }

    graphicList[m_direction]->draw(Gameplay::instance()->screen(),
                                   (int)(m_centerX - m_centerOffsetX - screenX),
                                   (int)(m_centerY - m_centerOffsetY - m_altitude - screenY));
}

