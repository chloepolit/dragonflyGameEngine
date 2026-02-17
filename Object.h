#pragma once

#include <string>
#include "Vector.h"
#include "Event.h"

const int MAX_ALTITUDE = 4; // Maximum object altitude (0-4)

namespace df {

// Solidness of object
enum Solidness {
    HARD,       // Object causes collisions and impedes movement
    SOFT,       // Object causes collisions but does NOT impede movement
    SPECTRAL,   // Object does not cause collisions
};

class Object {
private:
    int m_id;              // Unique identifier
    std::string m_type;    // Game-programmer-defined type
    Vector m_position;     // Position in game world
    int m_altitude;        // Altitude (layer): 0 to MAX_ALTITUDE
    float m_speed;         // Speed in direction
    Vector m_direction;    // Direction of object
    Solidness m_solidness; // Solidness of object
    std::string m_shape;   // Simple ASCII shape (used in draw())

public:
    // Construct Object. Add to WorldManager.
    Object();

    // Destroy Object. Remove from WorldManager.
    virtual ~Object();

    // Set Object ID
    void setId(int new_id);

    // Get Object ID
    int getId() const;

    // Set type identifier of Object
    void setType(std::string new_type);

    // Get type identifier of Object
    std::string getType() const;

    // Set position of Object
    void setPosition(Vector new_pos);

    // Get position of Object
    Vector getPosition() const;

    // Set altitude of Object [0, MAX_ALTITUDE]
    // Return 0 if ok, else -1
    int setAltitude(int new_altitude);

    // Return altitude of Object
    int getAltitude() const;

    // Set speed of Object
    void setSpeed(float speed);

    // Get speed of Object
    float getSpeed() const;

    // Set direction of Object
    void setDirection(Vector new_direction);

    // Get direction of Object
    Vector getDirection() const;

    // Set velocity (speed + direction) of Object
    void setVelocity(Vector new_velocity);

    // Get velocity (speed * direction) of Object
    Vector getVelocity() const;

    // Predict position based on speed and direction
    Vector predictPosition();

    // Return true if Object is HARD or SOFT (solid)
    bool isSolid() const;

    // Set solidness of Object
    // Return 0 if ok, else -1
    int setSolidness(Solidness new_solid);

    // Return solidness of Object
    Solidness getSolidness() const;

    // Handle event. Return 1 if handled, 0 if not.
    virtual int eventHandler(const Event *p_e);

    // Draw Object (default: draw shape string at position)
    virtual int draw();
};

} // end namespace df
