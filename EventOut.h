#pragma once

#include "Event.h"

const std::string OUT_EVENT = "df::out";

namespace df {

class EventOut : public Event {
public:
    // Create out-of-bounds event
    EventOut();
};

} // end namespace df
