#ifndef _INPUT_H_
#define _INPUT_H_

#include <map>
#include <string>

#include <SFML/Window.hpp>

// Input maps KeyCodes to InputActions
class Input {
public:
    Input(const sf::Input & input);

    enum Action {
        // Movement
        North, East, South, West,
        Jump,

        // Fighting
        Attack_1,

        // size of the enum
        InputActionCount,
    };

    // this enum maps our key codes to our library's key codes so that we can
    // avoid being dependent on a library in too many places
    enum KeyCode {
        kcA = sf::Key::A,
        kcB = sf::Key::B,
        kcC = sf::Key::C,
        kcD = sf::Key::D,
        kcE = sf::Key::E,
        kcF = sf::Key::F,
        kcG = sf::Key::G,
        kcH = sf::Key::H,
        kcI = sf::Key::I,
        kcJ = sf::Key::J,
        kcK = sf::Key::K,
        kcL = sf::Key::L,
        kcM = sf::Key::M,
        kcN = sf::Key::N,
        kcO = sf::Key::O,
        kcP = sf::Key::P,
        kcQ = sf::Key::Q,
        kcR = sf::Key::R,
        kcS = sf::Key::S,
        kcT = sf::Key::T,
        kcU = sf::Key::U,
        kcV = sf::Key::V,
        kcW = sf::Key::W,
        kcX = sf::Key::X,
        kcY = sf::Key::Y,
        kcZ = sf::Key::Z,
        kcNum0 = sf::Key::Num0,
        kcNum1 = sf::Key::Num1,
        kcNum2 = sf::Key::Num2,
        kcNum3 = sf::Key::Num3,
        kcNum4 = sf::Key::Num4,
        kcNum5 = sf::Key::Num5,
        kcNum6 = sf::Key::Num6,
        kcNum7 = sf::Key::Num7,
        kcNum8 = sf::Key::Num8,
        kcNum9 = sf::Key::Num9,
        kcEscape = sf::Key::Escape,
        kcLControl = sf::Key::LControl,
        kcLShift = sf::Key::LShift,
        kcLAlt = sf::Key::LAlt,
        kcLSystem = sf::Key::LSystem,
        kcRControl = sf::Key::RControl,
        kcRShift = sf::Key::RShift,
        kcRAlt = sf::Key::RAlt,
        kcRSystem = sf::Key::RSystem,
        kcMenu = sf::Key::Menu,
        kcLBracket = sf::Key::LBracket,
        kcRBracket = sf::Key::RBracket,
        kcSemiColon = sf::Key::SemiColon,
        kcComma = sf::Key::Comma,
        kcPeriod = sf::Key::Period,
        kcQuote = sf::Key::Quote,
        kcSlash = sf::Key::Slash,
        kckcBackSlash = sf::Key::BackSlash,
        kcTilde = sf::Key::Tilde,
        kcEqual = sf::Key::Equal,
        kcDash = sf::Key::Dash,
        kcSpace = sf::Key::Space,
        kcReturn = sf::Key::Return,
        kcBack = sf::Key::Back,
        kcTab = sf::Key::Tab,
        kcPageUp = sf::Key::PageUp,
        kcPageDown = sf::Key::PageDown,
        kcEnd = sf::Key::End,
        kcHome = sf::Key::Home,
        kcInsert = sf::Key::Insert,
        kcDelete = sf::Key::Delete,
        kcAdd = sf::Key::Add,
        kcSubtract = sf::Key::Subtract,
        kcMultiply = sf::Key::Multiply,
        kcDivide = sf::Key::Divide,
        kcLeft = sf::Key::Left,
        kcRight = sf::Key::Right,
        kcUp = sf::Key::Up,
        kcDown = sf::Key::Down,
        kcNumpad0 = sf::Key::Numpad0,
        kcNumpad1 = sf::Key::Numpad1,
        kcNumpad2 = sf::Key::Numpad2,
        kcNumpad3 = sf::Key::Numpad3,
        kcNumpad4 = sf::Key::Numpad4,
        kcNumpad5 = sf::Key::Numpad5,
        kcNumpad6 = sf::Key::Numpad6,
        kcNumpad7 = sf::Key::Numpad7,
        kcNumpad8 = sf::Key::Numpad8,
        kcNumpad9 = sf::Key::Numpad9,
        kcF1 = sf::Key::F1,
        kcF2 = sf::Key::F2,
        kcF3 = sf::Key::F3,
        kcF4 = sf::Key::F4,
        kcF5 = sf::Key::F5,
        kcF6 = sf::Key::F6,
        kcF7 = sf::Key::F7,
        kcF8 = sf::Key::F8,
        kcF9 = sf::Key::F9,
        kcF10 = sf::Key::F10,
        kcF11 = sf::Key::F11,
        kcF12 = sf::Key::F12,
        kcF13 = sf::Key::F13,
        kcF14 = sf::Key::F14,
        kcF15 = sf::Key::F15,
        kcPause = sf::Key::Pause,
    };

    // call this at the start of each frame.
    void refresh();

    // whether a key is depressed currently.
    bool state(Action key);
    // whether a key has just been pressed (like a key-down event).
    bool justPressed(Action key);

private:
    bool m_state[InputActionCount];
    bool m_lastState[InputActionCount];
    KeyCode m_map[InputActionCount];

    const sf::Input & m_input;
};

#endif
