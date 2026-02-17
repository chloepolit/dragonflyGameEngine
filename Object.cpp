#include "Object.h"
#include "WorldManager.h"
#include "DisplayManager.h"
#include "LogManager.h"

namespace df {

// Static counter for unique IDs
static int object_count = 0;

Object::Object()
    : m_id(++object_count)
    , m_type("Object")
    , m_position(0, 0)
    , m_altitude(MAX_ALTITUDE / 2)
    , m_speed(0.0f)
    , m_direction(0, 0)
    , m_solidness(HARD)
    , m_shape("*")
{
    WM.insertObject(this);
    LM.writeLog("Object::Object() - created object id %d", m_id);
}

Object::~Object() {
    LM.writeLog("Object::~Object() - destroying object id %d", m_id);
    WM.removeObject(this);
}

void Object::setId(int new_id) {
    m_id = new_id;
}

int Object::getId() const {
    return m_id;
}

void Object::setType(std::string new_type) {
    m_type = new_type;
}

std::string Object::getType() const {
    return m_type;
}

void Object::setPosition(Vector new_pos) {
    m_position = new_pos;
}

Vector Object::getPosition() const {
    return m_position;
}

// BUG FIX: Original returned 1 on error instead of -1
int Object::setAltitude(int new_altitude) {
    if (new_altitude >= 0 && new_altitude <= MAX_ALTITUDE) {
        m_altitude = new_altitude;
        return 0;
    }
    return -1;
}

int Object::getAltitude() const {
    return m_altitude;
}

void Object::setSpeed(float speed) {
    m_speed = speed;
}

float Object::getSpeed() const {
    return m_speed;
}

void Object::setDirection(Vector new_direction) {
    m_direction = new_direction;
}

Vector Object::getDirection() const {
    return m_direction;
}

void Object::setVelocity(Vector new_velocity) {
    m_speed = new_velocity.getMagnitude();
    Vector direction = new_velocity;
    direction.normalize();
    m_direction = direction;
}

Vector Object::getVelocity() const {
    Vector velocity = m_direction;
    velocity.scale(m_speed);
    return velocity;
}

// BUG FIX: Original predictPosition only added speed (scalar) to both x and y
// independently, ignoring direction entirely. Fixed to add velocity vector.
Vector Object::predictPosition() {
    Vector velocity = getVelocity();
    Vector new_pos(m_position.getX() + velocity.getX(),
                   m_position.getY() + velocity.getY());
    return new_pos;
}

bool Object::isSolid() const {
    return (m_solidness == HARD || m_solidness == SOFT);
}

// BUG FIX: Original always set m_solidness, then checked if it WASN'T equal
// to new_solid (which was always false since it just assigned it), so it
// always returned 0 even on invalid input. Now properly validates first.
int Object::setSolidness(Solidness new_solid) {
    if (new_solid != HARD && new_solid != SOFT && new_solid != SPECTRAL) {
        return -1;
    }
    m_solidness = new_solid;
    return 0;
}

Solidness Object::getSolidness() const {
    return m_solidness;
}

int Object::eventHandler(const Event */*p_e*/) {
    return 0; // Base class does not handle events
}

int Object::draw() {
    DM.drawString(m_position, m_shape, LEFT_JUSTIFIED, GREEN);
    return 0;
}

} // end namespace df
