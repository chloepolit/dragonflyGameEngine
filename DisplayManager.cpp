#include <SFML/Graphics.hpp>
#include <iostream>

#include "DisplayManager.h"
#include "LogManager.h"
#include "Color.h"
#include "Manager.h"
#include "Vector.h"

namespace df {

DisplayManager::DisplayManager() {
    setType("DisplayManager");
    m_p_window = nullptr;
    m_window_horizontal_chars  = WINDOW_HORIZONTAL_CHARS_DEFAULT;
    m_window_vertical_chars    = WINDOW_VERTICAL_CHARS_DEFAULT;
    m_window_horizontal_pixels = WINDOW_HORIZONTAL_PIXELS_DEFAULT;
    m_window_vertical_pixels   = WINDOW_VERTICAL_PIXELS_DEFAULT;
}

DisplayManager &DisplayManager::getInstance() {
    static DisplayManager instance;
    return instance;
}

int DisplayManager::startUp() {
    if (m_p_window != nullptr) {
        return 0; // already started
    }

    // SFML 3: VideoMode takes width/height as separate args (not initializer list)
    m_p_window = new sf::RenderWindow(
        sf::VideoMode({(unsigned int)WINDOW_HORIZONTAL_PIXELS_DEFAULT,
                       (unsigned int)WINDOW_VERTICAL_PIXELS_DEFAULT}),
        WINDOW_TITLE_DEFAULT);

    m_p_window->setMouseCursorVisible(false);
    m_p_window->setVerticalSyncEnabled(true);

    // Use relative font path so it works on any machine
    if (!m_font.openFromFile(FONT_FILE_DEFAULT)) {
        LM.writeLog("DisplayManager::startUp() - ERROR: could not load font '%s'",
                    FONT_FILE_DEFAULT.c_str());
        delete m_p_window;
        m_p_window = nullptr;
        return -1;
    }

    LM.writeLog("DisplayManager::startUp() - OK");
    return Manager::startUp();
}

void DisplayManager::shutDown() {
    if (m_p_window != nullptr) {
        m_p_window->close();
        delete m_p_window;
        m_p_window = nullptr;
    }
    Manager::shutDown();
    LM.writeLog("DisplayManager::shutDown() - OK");
}

int DisplayManager::swapBuffers() {
    if (m_p_window == nullptr) return -1;
    m_p_window->display();
    m_p_window->clear();
    return 0;
}

sf::RenderWindow *DisplayManager::getWindow() const {
    return m_p_window;
}

int DisplayManager::getHorizontal() const      { return m_window_horizontal_chars;  }
int DisplayManager::getVertical() const        { return m_window_vertical_chars;    }
int DisplayManager::getHorizontalPixels() const{ return m_window_horizontal_pixels; }
int DisplayManager::getVerticalPixels() const  { return m_window_vertical_pixels;   }

float DisplayManager::charHeight() const {
    return (float)getVerticalPixels() / (float)getVertical();
}

float DisplayManager::charWidth() const {
    return (float)getHorizontalPixels() / (float)getHorizontal();
}

Vector DisplayManager::spacesToPixels(Vector spaces) const {
    return Vector(spaces.getX() * charWidth(), spaces.getY() * charHeight());
}

Vector DisplayManager::pixelsToSpaces(Vector pixels) const {
    return Vector(pixels.getX() / charWidth(), pixels.getY() / charHeight());
}

int DisplayManager::drawCh(Vector world_pos, char ch, Color color) const {
    if (m_p_window == nullptr) return -1;

    Vector pixel_pos = spacesToPixels(world_pos);

    // Draw background rectangle so characters aren't transparent
    static sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(charWidth(), charHeight()));
    rectangle.setFillColor(WINDOW_BACKGROUND_COLOR_DEFAULT);
    rectangle.setPosition({pixel_pos.getX() - charWidth() / 10.0f,
                           pixel_pos.getY() + charHeight() / 5.0f});
    m_p_window->draw(rectangle);

    // SFML 3: sf::Text(font) takes const sf::Font& - no need for const_cast
    // We keep a static text and update it each call
    static sf::Text text(m_font);
    text.setString(ch);
    text.setStyle(sf::Text::Bold);

    if (charWidth() < charHeight())
        text.setCharacterSize((unsigned int)(charWidth() * 2));
    else
        text.setCharacterSize((unsigned int)(charHeight() * 2));

    switch (color) {
    case YELLOW:  text.setFillColor(sf::Color::Yellow);  break;
    case RED:     text.setFillColor(sf::Color::Red);     break;
    case BLACK:   text.setFillColor(sf::Color::Black);   break;
    case GREEN:   text.setFillColor(sf::Color::Green);   break;
    case BLUE:    text.setFillColor(sf::Color::Blue);    break;
    case MAGENTA: text.setFillColor(sf::Color::Magenta); break;
    case CYAN:    text.setFillColor(sf::Color::Cyan);    break;
    case WHITE:
    default:      text.setFillColor(sf::Color::White);   break;
    }

    text.setPosition({pixel_pos.getX(), pixel_pos.getY()});
    m_p_window->draw(text);
    return 0;
}

int DisplayManager::drawString(Vector pos, std::string str,
                               Justifications justif, Color color) const {
    Vector start = pos;
    switch (justif) {
    case CENTER_JUSTIFIED: start.setX(pos.getX() - (float)str.size() / 2.0f); break;
    case RIGHT_JUSTIFIED:  start.setX(pos.getX() - (float)str.size());        break;
    default: break;
    }
    for (int i = 0; i < (int)str.size(); i++) {
        drawCh(Vector(start.getX() + i, start.getY()), str[i], color);
    }
    return 0;
}

} // end namespace df
