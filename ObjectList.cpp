#include "ObjectList.h"
#include "Object.h"

namespace df {

ObjectList::ObjectList() : m_count(0) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        m_p_obj[i] = nullptr;
    }
}

int ObjectList::insert(Object *p_o) {
    if (isFull()) {
        return -1;
    }
    m_p_obj[m_count] = p_o;
    m_count++;
    return 0;
}

int ObjectList::remove(Object *p_o) {
    for (int i = 0; i < m_count; i++) {
        if (m_p_obj[i] == p_o) {
            // Shift remaining elements left
            for (int j = i; j < m_count - 1; j++) {
                m_p_obj[j] = m_p_obj[j + 1];
            }
            m_p_obj[m_count - 1] = nullptr;
            m_count--;
            return 0;
        }
    }
    return -1; // not found
}

void ObjectList::clear() {
    m_count = 0;
}

int ObjectList::getCount() const {
    return m_count;
}

bool ObjectList::isEmpty() const {
    return (m_count == 0);
}

bool ObjectList::isFull() const {
    return (m_count >= MAX_OBJECTS);
}

Object *ObjectList::operator[](int index) const {
    if (index < 0 || index >= m_count) {
        return nullptr;
    }
    return m_p_obj[index];
}

} // end namespace df
