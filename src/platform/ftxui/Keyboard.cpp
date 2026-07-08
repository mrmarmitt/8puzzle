#include "Keyboard.h"

#include <conio.h>

Key readKey() {
    if (!_kbhit()) {
        return Key::None;
    }

    const int ch = _getch();

    // Teclas estendidas (setas, F1..) chegam como prefixo 0/224 + código.
    if (ch == 0 || ch == 224) {
        if (!_kbhit()) {
            return Key::Other;
        }
        switch (_getch()) {
            case 72: return Key::Up;
            case 80: return Key::Down;
            case 75: return Key::Left;
            case 77: return Key::Right;
            default: return Key::Other;
        }
    }

    switch (ch) {
        case '\r': return Key::Enter;
        case 27:   return Key::Escape;
        default:   return Key::Other;
    }
}
