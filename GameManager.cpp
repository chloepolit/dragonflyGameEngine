#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "EventStep.h"
#include "Clock.h"
#include <thread>
#include <chrono>

namespace df {

GameManager::GameManager()
    : m_game_over(false)
    , m_frame_time(FRAME_TIME_DEFAULT)
{
    setType("GameManager");
}

GameManager &GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

int GameManager::startUp() {
    // Start LogManager first so we can log
    if (LM.startUp() != 0) {
        return -1;
    }

    LM.writeLog("GameManager::startUp() - starting up managers...");

    if (WM.startUp() != 0) {
        LM.writeLog("GameManager::startUp() - ERROR: WorldManager failed");
        return -1;
    }

    if (DM.startUp() != 0) {
        LM.writeLog("GameManager::startUp() - ERROR: DisplayManager failed");
        return -1;
    }

    if (IM.startUp() != 0) {
        LM.writeLog("GameManager::startUp() - ERROR: InputManager failed");
        return -1;
    }

    LM.writeLog("GameManager::startUp() - all managers started OK");
    return Manager::startUp();
}

void GameManager::shutDown() {
    LM.writeLog("GameManager::shutDown() - shutting down managers...");
    IM.shutDown();
    DM.shutDown();
    WM.shutDown();
    Manager::shutDown();
    LM.writeLog("GameManager::shutDown() - done");
    LM.shutDown();
}

void GameManager::run() {
    Clock clock;
    int step_count = 0;

    LM.writeLog("GameManager::run() - entering game loop at %d Hz",
                1000000 / m_frame_time);

    while (!m_game_over) {
        clock.delta(); // mark frame start

        // -- INPUT --
        IM.getInput();

        // -- UPDATE: send step event to all Objects --
        EventStep step(step_count++);
        onEvent(&step);

        // -- UPDATE: move objects, check collisions --
        WM.update();

        // -- DRAW: all objects draw themselves --
        WM.draw();

        // -- SWAP: refresh screen --
        DM.swapBuffers();

        // -- TIMING: sleep remaining time to hit target frame rate --
        long int elapsed = clock.split();
        long int sleep_time = m_frame_time - elapsed;
        if (sleep_time > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
        }

        // Drain elapsed time to start fresh for next frame
        clock.delta();
    }

    LM.writeLog("GameManager::run() - exited game loop after %d steps", step_count);
}

void GameManager::setGameOver(bool new_game_over) {
    m_game_over = new_game_over;
}

bool GameManager::getGameOver() const {
    return m_game_over;
}

void GameManager::setFrameTime(int new_frame_time) {
    m_frame_time = new_frame_time;
}

int GameManager::getFrameTime() const {
    return m_frame_time;
}

} // end namespace df
