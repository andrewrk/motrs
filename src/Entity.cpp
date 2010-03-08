#include "Entity.h"

#include "ResourceManager.h"
#include "Gameplay.h"
#include "Physics.h"

#include "Utils.h"

#include <cmath>

Entity::Entity():
    m_shape(Shapeless),
    m_centerX(0.0), m_centerY(0.0), m_radius(0),
    m_velocityX(0.0), m_velocityY(0.0),
    m_layer(0),
    m_altitude(0.0), m_altitudeVelocity(0.0),
    m_speed(0), m_mass(1),
    m_direction(Center), m_movementMode(Stand),
    m_centerOffsetX(0), m_centerOffsetY(0),
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
    if (version != 7) {
        std::cerr << "Unsupported Entity version: " << version << std::endl;
        return NULL;
    }

    Shape shape = (Shape)Utils::readInt(&cursor);
    double centerOffsetX = 0.0;
    double centerOffsetY = 0.0;
    double radius = 0.0;
    switch (shape) {
    case Shapeless:
        break;
    case Circle:
    case Square:
        centerOffsetX = (double)Utils::readInt(&cursor);
        centerOffsetY = (double)Utils::readInt(&cursor);
        radius = (double)Utils::readInt(&cursor);
        break;
    default:
        std::cerr << "Unknown shape: " << shape << "." << std::endl;
        return NULL;
    }

    double speed = Utils::readDouble(&cursor);
    double mass = Utils::readDouble(&cursor);

    Entity * entity = new Entity(shape, radius, centerOffsetX, centerOffsetY, speed, mass);
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

Entity::Entity(Shape shape, double radius, double centerOffsetX, double centerOffsetY, double speed, double mass) :
    m_shape(shape),
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

Tile::PhysicalPresence Entity::minPhysicalPresence() {
    switch (m_movementMode) {
    case Stand:
    case Walk:
    case Run:
        return Tile::ppRail;
    case JumpUp:
    case Falling:
        return Tile::ppEmbrasure;
    default:
        assert(false);
    }
}

void Entity::resolveCollision(Entity * other) {
    Entity * entity1 = NULL, * entity2 = NULL;
    double dx = 0.0, dy = 0.0;
    switch (this->m_shape) {
    case Shapeless:
        return;
    case Circle:
        switch (other->m_shape) {
        case Shapeless:
            return;
        case Circle:
            entity1 = this;
            entity2 = other;
            Physics::circleAndCircle(entity1->intendedCenterX(), entity1->intendedCenterY(), entity1->radius(), entity2->intendedCenterX(), entity2->intendedCenterY(), entity2->radius(), dx, dy);
            break;
        case Square:
            entity1 = other;
            entity2 = this;
            Physics::squareAndCircle(entity1->intendedCenterX(), entity1->intendedCenterY(), entity1->radius(), entity2->intendedCenterX(), entity2->intendedCenterY(), entity2->radius(), dx, dy);
            break;
        default:
            assert(false);
            break;
        }
        break;
    case Square:
        switch (other->m_shape) {
        case Shapeless:
            return;
        case Circle:
            entity1 = this;
            entity2 = other;
            Physics::squareAndCircle(entity1->intendedCenterX(), entity1->intendedCenterY(), entity1->radius(), entity2->intendedCenterX(), entity2->intendedCenterY(), entity2->radius(), dx, dy);
            break;
        case Square:
            entity1 = this;
            entity2 = other;
            Physics::squareAndSquare(entity1->intendedCenterX(), entity1->intendedCenterY(), entity1->radius(), entity2->intendedCenterX(), entity2->intendedCenterY(), entity2->radius(), dx, dy);
            break;
        default:
            assert(false);
            break;
        }
        break;
    default:
        assert(false);
        break;
    }

    if (Utils::isZero(dx) && Utils::isZero(dy))
        return;

    // resolve momentum collision
    double distance = std::sqrt(dx * dx + dy * dy);
    double normalX = dx / distance, normalY = dy / distance;
    double mass1 = entity1->mass(), mass2 = entity2->mass();
    double totalMass = mass1 + mass2;
    double push1 = -distance * mass2 / totalMass, push2 = distance * mass1 / totalMass;
    entity1->setVelocity(entity1->velocityX() + push1 * normalX, entity1->velocityY() + push1 * normalY);
    entity2->setVelocity(entity2->velocityX() + push2 * normalX, entity2->velocityY() + push2 * normalY);
}

void Entity::draw(double screenX, double screenY) {
    Graphic** graphicList;
    switch (m_movementMode) {
    case Stand: graphicList = m_standing; break;
    case Walk: graphicList = m_walking; break;
    case Run: graphicList = m_running; break;
    case JumpUp: graphicList = m_running; break;
    case Falling: graphicList = m_standing; break;
    default: assert(false);
    }
    // override with sequence graphics
    switch (m_currentSequence) {
    case None: break; // leave it alone
    case Sword: graphicList = m_sword; break;
    default: assert(false);
    }

    graphicList[m_direction]->draw(Gameplay::instance()->screen(),
                                   (int)(m_centerX - m_centerOffsetX - screenX),
                                   (int)(m_centerY - m_centerOffsetY - m_altitude - screenY));
}

