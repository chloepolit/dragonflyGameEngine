#pragma once

#include <SFML/Graphics.hpp>
#include "Color.h"
#include "Manager.h"
#include "Vector.h"

// Window defaults
const int WINDOW_HORIZONTAL_PIXELS_DEFAULT = 1024;
const int WINDOW_VERTICAL_PIXELS_DEFAULT   = 768;
const int WINDOW_HORIZONTAL_CHARS_DEFAULT  = 80;
const int WINDOW_VERTICAL_CHARS_DEFAULT    = 24;
const sf::Color WINDOW_BACKGROUND_COLOR_DEFAULT = sf::Color::Black;
const std::string WINDOW_TITLE_DEFAULT = "Dragonfly";
const std::string FONT_FILE_DEFAULT    = "df-font.ttf";

#define DM df::DisplayManager::getInstance()

namespace df {

enum Justifications {
    LEFT_JUSTIFIED,
    CENTER_JUSTIFIED,
    RIGHT_JUSTIFIED,
};

class DisplayManager : public Manager {
private:
    DisplayManager();                              // Private (singleton)
    DisplayManager(DisplayManager const &);        // No copy
    void operator=(DisplayManager const &);        // No assign

    sf::Font m_font;                   // Font used for ASCII graphics
    sf::RenderWindow *m_p_window;      // Pointer to SFML window
    int m_window_horizontal_pixels;    // Horizontal pixels in window
    int m_window_vertical_pixels;      // Vertical pixels in window
    int m_window_horizontal_chars;     // Horizontal ASCII spaces in window
    int m_window_vertical_chars;       // Vertical ASCII spaces in window

public:
    // Get the one and only instance of the DisplayManager
    static DisplayManager &getInstance();

    // Open graphics window, ready for text-based display
    // Return 0 if ok, else -1
    int startUp();

    // Close graphics window
    void shutDown();

    // Return window's horizontal maximum (in characters)
    int getHorizontal() const;

    // Return window's vertical maximum (in characters)
    int getVertical() const;

    // Return window's horizontal maximum (in pixels)
    int getHorizontalPixels() const;

    // Return window's vertical maximum (in pixels)
    int getVerticalPixels() const;

    // Render current window buffer to screen
    // Return 0 if ok, else -1
    int swapBuffers();

    // Return pointer to SFML graphics window
    sf::RenderWindow *getWindow() const;

    // Draw a character at window location (x,y) with color
    // Return 0 if ok, else -1
    int drawCh(Vector world_pos, char ch, Color color) const;

    // Draw string at position with justification and color
    // Return 0 if ok, else -1
    int drawString(Vector pos, std::string str, Justifications justif, Color color) const;

    // Compute character height in pixels
    float charHeight() const;

    // Compute character width in pixels
    float charWidth() const;

    // Convert ASCII spaces (x,y) to window pixels (x,y)
    Vector spacesToPixels(Vector spaces) const;

    // Convert window pixels (x,y) to ASCII spaces (x,y)
    Vector pixelsToSpaces(Vector pixels) const;
};

} // end namespace df
