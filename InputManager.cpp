#include "InputManager.h"
#include "DisplayManager.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "LogManager.h"
#include "Manager.h"
#include "WorldManager.h"
#include "Object.h"
#include <SFML/Graphics.hpp>
#include <optional>

namespace df {

InputManager::InputManager() {
    setType("InputManager");
}

InputManager &InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

int InputManager::startUp() {
    if (!DM.isStarted()) {
        LM.writeLog("InputManager::startUp() - DisplayManager not started");
        return -1;
    }
    LM.writeLog("InputManager::startUp() - OK");
    return Manager::startUp();
}

void InputManager::shutDown() {
    Manager::shutDown();
    LM.writeLog("InputManager::shutDown() - OK");
}

// Helper: broadcast a df event to all world objects
static void broadcast(const df::Event *p_e) {
    ObjectList all = WM.getAllObjects();
    for (int i = 0; i < all.getCount(); i++) {
        all[i]->eventHandler(p_e);
    }
}

// SFML 3 completely redesigned its event system:
// - pollEvent() returns std::optional<sf::Event> (no out-param)
// - Events accessed via event->getIf<sf::Event::KeyPressed>() etc.
// - Mouse buttons are sf::Mouse::Button::Left/Right/Middle
void InputManager::getInput() const {
    sf::RenderWindow *p_window = DM.getWindow();
    if (p_window == nullptr) return;

    while (auto event = p_window->pollEvent()) {

        // Window closed
        if (event->is<sf::Event::Closed>()) {
            p_window->close();
        }

        // Key pressed
        else if (const auto *kp = event->getIf<sf::Event::KeyPressed>()) {
            EventKeyboard ek;
            ek.setKeyboardAction(KEY_PRESSED);
            ek.setKey(EventKeyboard::convertFromSFML(kp->code));
            broadcast(&ek);
        }

        // Key released
        else if (const auto *kr = event->getIf<sf::Event::KeyReleased>()) {
            EventKeyboard ek;
            ek.setKeyboardAction(KEY_RELEASED);
            ek.setKey(EventKeyboard::convertFromSFML(kr->code));
            broadcast(&ek);
        }

        // Mouse button pressed
        else if (const auto *mb = event->getIf<sf::Event::MouseButtonPressed>()) {
            EventMouse em;
            em.setMouseAction(CLICKED);
            if (mb->button == sf::Mouse::Button::Left)
                em.setMouseButton(df::LEFT);
            else if (mb->button == sf::Mouse::Button::Right)
                em.setMouseButton(df::RIGHT);
            else
                em.setMouseButton(df::MIDDLE);
            Vector mouse_pos = DM.pixelsToSpaces(
                Vector((float)mb->position.x, (float)mb->position.y));
            em.setMousePosition(mouse_pos);
            broadcast(&em);
        }

        // Mouse moved
        else if (const auto *mm = event->getIf<sf::Event::MouseMoved>()) {
            EventMouse em;
            em.setMouseAction(MOVED);
            em.setMouseButton(df::UNDEFINED_MOUSE_BUTTON);
            Vector mouse_pos = DM.pixelsToSpaces(
                Vector((float)mm->position.x, (float)mm->position.y));
            em.setMousePosition(mouse_pos);
            broadcast(&em);
        }
    }
}

} // end namespace df
