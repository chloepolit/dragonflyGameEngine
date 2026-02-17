#pragma once

#include "Manager.h"
#include "ObjectList.h"

#define WM df::WorldManager::getInstance()

namespace df {

class WorldManager : public Manager {
private:
    ObjectList m_updates;       // All active game objects
    ObjectList m_deletions;     // Objects marked for deletion

    WorldManager();                             // Private (singleton)
    WorldManager(WorldManager const &);         // No copy
    void operator=(WorldManager const &);       // No assign

public:
    // Get the one and only instance of the WorldManager
    static WorldManager &getInstance();

    // Start up WorldManager
    // Return 0 if ok, else -1
    int startUp();

    // Shut down WorldManager: delete all objects
    void shutDown();

    // Insert Object into world
    // Return 0 if ok, else -1
    int insertObject(Object *p_o);

    // Remove Object from world
    // Return 0 if ok, else -1
    int removeObject(Object *p_o);

    // Return list of all Objects in world
    ObjectList getAllObjects() const;

    // Return list of Objects matching given type
    ObjectList objectsOfType(std::string type) const;

    // Mark Object for deferred deletion
    // Return 0 if ok, else -1
    int markForDelete(Object *p_o);

    // Update game world:
    //   - Move objects by velocity
    //   - Detect and send collision events
    //   - Send out-of-bounds events
    //   - Delete marked objects
    void update();

    // Draw all Objects (ordered by altitude)
    void draw();

    // Horizontal boundary (in spaces)
    int getHorizontal() const;

    // Vertical boundary (in spaces)
    int getVertical() const;
};

} // end namespace df
