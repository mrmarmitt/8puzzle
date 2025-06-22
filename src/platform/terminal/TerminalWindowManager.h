#pragma once

#include <windows.h>

#include "platform/IWindowManager.h"

class TerminalWindowManager : public IWindowManager {
    HANDLE m_hConsole;
    COORD m_homeCoords;

public:
    TerminalWindowManager();
    void init() override;
    void update() override;
    void cleanup() override;
};
