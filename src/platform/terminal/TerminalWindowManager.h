#pragma once

#include <windows.h>

#include "../../8puzzle/engine/IWindowManager.h"

class TerminalWindowManager : public IWindowManager {
    HANDLE m_hConsole;
    COORD m_homeCoords;

public:
    TerminalWindowManager();
    void init() override;
    void update() override;
    void cleanup() override;
    static auto gotoXY(int x, int y) -> void;
};
