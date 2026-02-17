#pragma once

#include <string>

// Forward declare Event to avoid circular dependency
namespace df { class Event; }

namespace df {

class Manager {
private:
    std::string m_type;    // Manager type identifier
    bool m_is_started;     // true when started successfully

protected:
    // Set type identifier of Manager
    void setType(std::string type);

public:
    Manager();
    virtual ~Manager();

    // Get type identifier of Manager
    std::string getType() const;

    // Start up Manager
    // Return 0 if ok, else negative number
    virtual int startUp();

    // Shut down Manager
    virtual void shutDown();

    // Return true when startUp() was executed ok, else false
    bool isStarted() const;

    // Send event to all Objects registered with WorldManager.
    // Return number of objects that handled the event.
    int onEvent(const Event *p_event) const;
};

} // end namespace df
