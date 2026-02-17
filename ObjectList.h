#pragma once

const int MAX_OBJECTS = 1000;

// Forward declare to break circular dependency with WorldManager
namespace df { class Object; }

namespace df {

class ObjectList {
private:
    int m_count;                    // Count of objects in list
    Object *m_p_obj[MAX_OBJECTS];   // Array of pointers to Objects

public:
    // Default constructor
    ObjectList();

    // Insert object pointer in list
    // Return 0 if ok, else -1
    int insert(Object *p_o);

    // Remove object pointer from list
    // Return 0 if found and removed, else -1
    int remove(Object *p_o);

    // Clear list (set count to 0)
    void clear();

    // Return count of objects in list
    int getCount() const;

    // Return true if list is empty
    bool isEmpty() const;

    // Return true if list is full
    bool isFull() const;

    // Index into list (no bounds check for speed)
    Object *operator[](int index) const;
};

} // end namespace df
