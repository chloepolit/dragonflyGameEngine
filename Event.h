#pragma once

#include <string>

// Defined outside namespace for global access
const std::string UNDEFINED_EVENT = "df::undefined";

namespace df {

class Event {
private:
    std::string m_event_type; // Holds event type

public:
    // Create base event with undefined type
    Event();

    // Virtual destructor for proper polymorphism
    virtual ~Event();

    // Set event type
    void setType(std::string new_type);

    // Get event type
    std::string getType() const;
};

} // end namespace df
