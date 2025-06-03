#pragma once

#include <windows.h>
#include "../IWindowManager.h"

class WindowManager : public IWindowManager {
private:
    HANDLE m_hConsole;
    COORD m_homeCoords;

public:
    WindowManager();
    ~WindowManager();
    void init() override;
    void update() override;
    void cleanup() override;
};