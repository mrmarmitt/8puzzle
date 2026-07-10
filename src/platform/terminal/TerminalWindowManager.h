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
    // Vazio (cengine 0.5.0): terminal puro não tem conceito de present — as
    // cenas escrevem direto no console durante o draw.
    void present() override {}
    void cleanup() override;
    static auto gotoXY(int x, int y) -> void;
};
