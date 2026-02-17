#include "EventMouse.h"

namespace df {

// BUG FIX: Original constructor used "df::Event(MSE_EVENT)" which doesn't
// call the parent constructor - it constructs a temporary. Fixed to use
// proper initializer list and setType().
EventMouse::EventMouse()
    : m_mouse_action(UNDEFINED_MOUSE_ACTIONS)
    , m_mouse_button(UNDEFINED_MOUSE_BUTTON)
    , m_mouse_xy()
{
    setType(MSE_EVENT);
}

void EventMouse::setMouseAction(EventMouseAction new_mouse_action) {
    m_mouse_action = new_mouse_action;
}

EventMouseAction EventMouse::getMouseAction() const {
    return m_mouse_action;
}

void EventMouse::setMouseButton(df::Button new_mouse_button) {
    m_mouse_button = new_mouse_button;
}

df::Button EventMouse::getMouseButton() const {
    return m_mouse_button;
}

void EventMouse::setMousePosition(Vector new_mouse_xy) {
    m_mouse_xy = new_mouse_xy;
}

Vector EventMouse::getMousePosition() const {
    return m_mouse_xy;
}

} // end namespace df
