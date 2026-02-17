#include "WorldManager.h"
#include "LogManager.h"
#include "DisplayManager.h"
#include "EventCollision.h"
#include "EventOut.h"
#include "Object.h"

namespace df {

WorldManager::WorldManager() {
    setType("WorldManager");
}

WorldManager &WorldManager::getInstance() {
    static WorldManager instance;
    return instance;
}

int WorldManager::startUp() {
    m_updates.clear();
    m_deletions.clear();
    LM.writeLog("WorldManager::startUp() - OK");
    return Manager::startUp();
}

void WorldManager::shutDown() {
    // Delete all objects
    ObjectList all = m_updates;
    for (int i = 0; i < all.getCount(); i++) {
        delete all[i];
    }
    m_updates.clear();
    m_deletions.clear();
    Manager::shutDown();
    LM.writeLog("WorldManager::shutDown() - OK");
}

int WorldManager::insertObject(Object *p_o) {
    return m_updates.insert(p_o);
}

int WorldManager::removeObject(Object *p_o) {
    return m_updates.remove(p_o);
}

ObjectList WorldManager::getAllObjects() const {
    return m_updates;
}

ObjectList WorldManager::objectsOfType(std::string type) const {
    ObjectList list;
    for (int i = 0; i < m_updates.getCount(); i++) {
        if (m_updates[i]->getType() == type) {
            list.insert(m_updates[i]);
        }
    }
    return list;
}

int WorldManager::markForDelete(Object *p_o) {
    // Avoid duplicate marks
    for (int i = 0; i < m_deletions.getCount(); i++) {
        if (m_deletions[i] == p_o) {
            return 0; // already marked
        }
    }
    return m_deletions.insert(p_o);
}

// Move a single object, checking collisions and out-of-bounds.
static void moveObject(Object *p_o, Vector new_pos) {
    // Check collisions with all other solid objects
    if (p_o->isSolid()) {
        ObjectList all = WM.getAllObjects();
        for (int i = 0; i < all.getCount(); i++) {
            Object *p_temp = all[i];
            if (p_temp == p_o) continue;
            if (!p_temp->isSolid()) continue;

            // Simple point collision check
            if (static_cast<int>(new_pos.getX()) == static_cast<int>(p_temp->getPosition().getX()) &&
                static_cast<int>(new_pos.getY()) == static_cast<int>(p_temp->getPosition().getY())) {

                // Send collision event to both objects
                EventCollision ec(p_o, p_temp, new_pos);
                p_o->eventHandler(&ec);
                p_temp->eventHandler(&ec);

                // HARD objects block movement
                if (p_o->getSolidness() == HARD && p_temp->getSolidness() == HARD) {
                    return; // don't move
                }
            }
        }
    }

    // Move object
    p_o->setPosition(new_pos);

    // Check out of bounds
    int h = WM.getHorizontal();
    int v = WM.getVertical();
    float x = new_pos.getX();
    float y = new_pos.getY();
    if (x < 0 || x >= h || y < 0 || y >= v) {
        EventOut eo;
        p_o->eventHandler(&eo);
    }
}

void WorldManager::update() {
    // Move all objects by velocity
    for (int i = 0; i < m_updates.getCount(); i++) {
        Object *p_o = m_updates[i];
        Vector vel = p_o->getVelocity();
        if (vel.getX() != 0.0f || vel.getY() != 0.0f) {
            Vector new_pos = p_o->predictPosition();
            moveObject(p_o, new_pos);
        }
    }

    // Delete objects marked for deletion
    for (int i = 0; i < m_deletions.getCount(); i++) {
        delete m_deletions[i];
    }
    m_deletions.clear();
}

void WorldManager::draw() {
    // Draw objects in altitude order (lowest first)
    for (int alt = 0; alt <= MAX_ALTITUDE; alt++) {
        for (int i = 0; i < m_updates.getCount(); i++) {
            if (m_updates[i]->getAltitude() == alt) {
                m_updates[i]->draw();
            }
        }
    }
}

int WorldManager::getHorizontal() const {
    return DM.getHorizontal();
}

int WorldManager::getVertical() const {
    return DM.getVertical();
}

} // end namespace df
