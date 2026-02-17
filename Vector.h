#pragma once

namespace df {

class Vector {
private:
    float m_x; // Horizontal component
    float m_y; // Vertical component

public:
    // Default constructor (0,0)
    Vector();

    // Constructor with values
    Vector(float init_x, float init_y);

    // Set x component
    void setX(float new_x);

    // Get x component
    float getX() const;

    // Set y component
    void setY(float new_y);

    // Get y component
    float getY() const;

    // Set both x,y components
    void setXY(float new_x, float new_y);

    // Return magnitude of vector
    float getMagnitude() const;

    // Normalize vector (make unit length). No change if magnitude is 0.
    void normalize();

    // Scale vector by scale factor
    void scale(float s);

    // Add vector to this vector
    Vector operator+(const Vector &other) const;

    // Subtract vector from this vector
    Vector operator-(const Vector &other) const;

    // Equality comparison
    bool operator==(const Vector &other) const;

    // Inequality comparison
    bool operator!=(const Vector &other) const;
};

} // end namespace df
