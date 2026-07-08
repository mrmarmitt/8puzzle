#include "Keyboard.h"

#include <cctype>
#include <conio.h>

KeyEvent readKey() {
    if (!_kbhit()) {
        return {};
    }

    const int ch = _getch();

    // Teclas estendidas (setas, F1..) chegam como prefixo 0/224 + código.
    if (ch == 0 || ch == 224) {
        if (!_kbhit()) {
            return {Key::Other};
        }
        switch (_getch()) {
            case 72: return {Key::Up};
            case 80: return {Key::Down};
            case 75: return {Key::Left};
            case 77: return {Key::Right};
            default: return {Key::Other};
        }
    }

    switch (ch) {
        case '\r': return {Key::Enter};
        case 27:   return {Key::Escape};
        case 8:    return {Key::Backspace};
        default:
            if (std::isprint(static_cast<unsigned char>(ch))) {
                return {Key::Char, static_cast<char>(ch)};
            }
            return {Key::Other};
    }
}
