#include "Clock.h"

namespace df {

Clock::Clock() {
    m_previous_time = std::chrono::steady_clock::now();
}

// Return elapsed microseconds since last delta() call, then reset timer.
long int Clock::delta() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        now - m_previous_time).count();
    m_previous_time = now; // reset
    return static_cast<long int>(elapsed);
}

// Return elapsed microseconds since last delta() call WITHOUT resetting.
long int Clock::split() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        now - m_previous_time).count();
    return static_cast<long int>(elapsed);
}

} // end namespace df
