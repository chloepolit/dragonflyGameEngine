#include "Manager.h"
#include "WorldManager.h"
#include "ObjectList.h"
#include "Object.h"

namespace df {

Manager::Manager() : m_type("Manager"), m_is_started(false) {}

Manager::~Manager() {}

void Manager::setType(std::string type) {
    m_type = type;
}

std::string Manager::getType() const {
    return m_type;
}

int Manager::startUp() {
    m_is_started = true;
    return 0;
}

void Manager::shutDown() {
    m_is_started = false;
}

bool Manager::isStarted() const {
    return m_is_started;
}

// Send event to all Objects in the game world.
// Return count of objects that handled the event.
int Manager::onEvent(const Event *p_event) const {
    int count = 0;
    ObjectList all = WM.getAllObjects();
    for (int i = 0; i < all.getCount(); i++) {
        count += all[i]->eventHandler(p_event);
    }
    return count;
}

} // end namespace df
