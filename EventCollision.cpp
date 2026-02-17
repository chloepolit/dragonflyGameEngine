#include "EventCollision.h"
#include "Object.h"

namespace df {

// BUG FIX: Original default constructor used:
//   Object* m_p_obj1 = new Object();
// which declares a LOCAL variable (shadowing the member), leaking memory
// and leaving the members uninitialized.
EventCollision::EventCollision()
    : m_pos()
    , m_p_obj1(nullptr)
    , m_p_obj2(nullptr)
{
    setType(COLLISION_EVENT);
}

// BUG FIX: Same issue in parameterized constructor - local vars shadowed members.
EventCollision::EventCollision(Object *p_o1, Object *p_o2, Vector p)
    : m_pos(p)
    , m_p_obj1(p_o1)
    , m_p_obj2(p_o2)
{
    setType(COLLISION_EVENT);
}

void EventCollision::setObject1(Object *p_new_o1) {
    m_p_obj1 = p_new_o1;
}

Object *EventCollision::getObject1() const {
    return m_p_obj1;
}

void EventCollision::setObject2(Object *p_new_o2) {
    m_p_obj2 = p_new_o2;
}

Object *EventCollision::getObject2() const {
    return m_p_obj2;
}

void EventCollision::setPosition(Vector new_pos) {
    m_pos = new_pos;
}

Vector EventCollision::getPosition() const {
    return m_pos;
}

} // end namespace df
