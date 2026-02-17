#pragma once

#include "Manager.h"

#define GM df::GameManager::getInstance()

// Default game loop target (steps per second)
const int FRAME_TIME_DEFAULT = 33333; // ~30 Hz in microseconds

namespace df {

class GameManager : public Manager {
private:
    bool m_game_over;   // True when game loop should end
    int m_frame_time;   // Target microseconds per frame

    GameManager();                           // Private (singleton)
    GameManager(GameManager const &);        // No copy
    void operator=(GameManager const &);     // No assign

public:
    // Get the one and only instance of the GameManager
    static GameManager &getInstance();

    // Start up all managers (Log, World, Display, Input)
    // Return 0 if ok, else -1
    int startUp();

    // Shut down all managers in reverse order
    void shutDown();

    // Run the game loop until game over
    void run();

    // Set game over flag to stop the loop
    void setGameOver(bool new_game_over = true);

    // Get game over state
    bool getGameOver() const;

    // Set target frame time (microseconds)
    void setFrameTime(int new_frame_time);

    // Get target frame time (microseconds)
    int getFrameTime() const;
};

} // end namespace df
