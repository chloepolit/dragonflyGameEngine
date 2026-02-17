#pragma once
// The clock, for timing (such as in game loop)
// Cross-platform implementation using <chrono>

#include <chrono>

namespace df {

class Clock {
private:
    // Time point of last delta() call
    std::chrono::steady_clock::time_point m_previous_time;

public:
    // Sets previous time to current time
    Clock();

    // Return time elapsed since delta() was last called, -1 if error
    // Resets previous time
    // Units are microseconds
    long int delta();

    // Return time elapsed since delta() was last called, -1 if error
    // Does NOT reset previous time
    // Units are microseconds
    long int split() const;
};

} // end namespace df
