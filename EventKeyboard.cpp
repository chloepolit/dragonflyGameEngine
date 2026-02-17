#include "EventKeyboard.h"

namespace df {

EventKeyboard::EventKeyboard()
    : m_key_val(UNDEFINED_KEY)
    , m_keyboard_action(UNDEFINED_KEYBOARD_ACTION)
{
    setType(KEYBOARD_EVENT);
}

void EventKeyboard::setKey(df::Key new_key)            { m_key_val = new_key; }
df::Key EventKeyboard::getKey() const                  { return m_key_val; }
void EventKeyboard::setKeyboardAction(EventKeyboardAction a) { m_keyboard_action = a; }
EventKeyboardAction EventKeyboard::getKeyboardAction() const { return m_keyboard_action; }

// SFML 3: keyboard keys are scoped enum sf::Keyboard::Key::A (same values,
// but must match the new scoped names). Mouse/keyboard APIs also changed,
// but key *values* are structurally the same as SFML 2 for letters/numbers.
// BUG FIX from original: D key was mapped to df::E (copy-paste error).
df::Key EventKeyboard::convertFromSFML(sf::Keyboard::Key key) {
    using K = sf::Keyboard::Key; // shorthand
    switch (key) {
    case K::Space:     return df::SPACE;
    case K::Enter:     return df::RETURN;
    case K::Escape:    return df::ESCAPE;
    case K::Tab:       return df::TAB;
    case K::Left:      return df::LEFTARROW;
    case K::Right:     return df::RIGHTARROW;
    case K::Up:        return df::UPARROW;
    case K::Down:      return df::DOWNARROW;
    case K::Pause:     return df::PAUSE;
    case K::Subtract:  return df::MINUS;
    case K::Add:       return df::PLUS;
    case K::Grave:     return df::TILDE;
    case K::Period:    return df::PERIOD;
    case K::Comma:     return df::COMMA;
    case K::Slash:     return df::SLASH;
    case K::LControl:  return df::LEFTCONTROL;
    case K::RControl:  return df::RIGHTCONTROL;
    case K::LShift:    return df::LEFTSHIFT;
    case K::RShift:    return df::RIGHTSHIFT;
    case K::F1:        return df::F1;
    case K::F2:        return df::F2;
    case K::F3:        return df::F3;
    case K::F4:        return df::F4;
    case K::F5:        return df::F5;
    case K::F6:        return df::F6;
    case K::F7:        return df::F7;
    case K::F8:        return df::F8;
    case K::F9:        return df::F9;
    case K::F10:       return df::F10;
    case K::A:         return df::A;
    case K::B:         return df::B;
    case K::C:         return df::C;
    case K::D:         return df::D;   // BUG FIX: was returning df::E
    case K::E:         return df::E;
    case K::F:         return df::F;
    case K::G:         return df::G;
    case K::H:         return df::H;
    case K::I:         return df::I;
    case K::J:         return df::J;
    case K::K:         return df::K;
    case K::L:         return df::L;
    case K::M:         return df::M;
    case K::N:         return df::N;
    case K::O:         return df::O;
    case K::P:         return df::P;
    case K::Q:         return df::Q;
    case K::R:         return df::R;
    case K::S:         return df::S;
    case K::T:         return df::T;
    case K::U:         return df::U;
    case K::V:         return df::V;
    case K::W:         return df::W;
    case K::X:         return df::X;
    case K::Y:         return df::Y;
    case K::Z:         return df::Z;
    case K::Num0:      return df::NUM0;
    case K::Num1:      return df::NUM1;
    case K::Num2:      return df::NUM2;
    case K::Num3:      return df::NUM3;
    case K::Num4:      return df::NUM4;
    case K::Num5:      return df::NUM5;
    case K::Num6:      return df::NUM6;
    case K::Num7:      return df::NUM7;
    case K::Num8:      return df::NUM8;
    case K::Num9:      return df::NUM9;
    default:           return df::UNDEFINED_KEY;
    }
}

} // end namespace df
