#pragma once

#include "Event.h"
#include "Vector.h"

const std::string MSE_EVENT = "df::mouse";

// Set of mouse actions recognized by Dragonfly
enum EventMouseAction {
    UNDEFINED_MOUSE_ACTIONS = -1,
    CLICKED,
    MOVED,
};

namespace df {

// Set of mouse buttons recognized by Dragonfly
enum Button {
    UNDEFINED_MOUSE_BUTTON = -1,
    LEFT,
    RIGHT,
    MIDDLE,
};

class EventMouse : public Event {
private:
    EventMouseAction m_mouse_action; // Mouse action
    df::Button m_mouse_button;       // Mouse button
    Vector m_mouse_xy;               // Mouse (x,y) coordinates

public:
    EventMouse();

    // Set mouse event's action
    void setMouseAction(EventMouseAction new_mouse_action);

    // Get mouse event's action
    EventMouseAction getMouseAction() const;

    // Set mouse event's button
    void setMouseButton(df::Button new_mouse_button);

    // Get mouse event's button
    df::Button getMouseButton() const;

    // Set mouse event's position
    void setMousePosition(Vector new_mouse_xy);

    // Get mouse event's position
    Vector getMousePosition() const;
};

} // end namespace df
