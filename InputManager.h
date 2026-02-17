#pragma once

#include "Manager.h"

#define IM df::InputManager::getInstance()

namespace df {

class InputManager : public Manager {
private:
    InputManager();                              // Private (singleton)
    InputManager(InputManager const &);          // No copy
    void operator=(InputManager const &);        // No assign

public:
    // Get the one and only instance of the InputManager
    static InputManager &getInstance();

    // Get window ready to capture input
    // Return 0 if ok, else -1
    int startUp();

    // Revert back to normal window mode
    void shutDown();

    // Get input from keyboard and mouse, pass event to all Objects
    void getInput() const;
};

} // end namespace df
