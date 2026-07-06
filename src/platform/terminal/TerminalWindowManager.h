#pragma once

#include <windows.h>

#include <cengine/core/IWindowManager.hpp>

class TerminalWindowManager : public cengine::core::IWindowManager {
    HANDLE m_hConsole;
    COORD m_homeCoords;

public:
    TerminalWindowManager();
    void init() override;
    void update() override;
    void cleanup() override;
    static auto gotoXY(int x, int y) -> void;
};
