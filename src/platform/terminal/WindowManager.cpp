#include <iostream>
#include "WindowManager.h"

using namespace std;

WindowManager::WindowManager() :
    m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE)), 
    m_homeCoords({0, 0}) {
}

WindowManager::~WindowManager() {
    cleanup();
}

void WindowManager::init() {

}

void WindowManager::update() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;

    // Obt�m o tamanho do buffer do console
    if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Preenche todo o console com espa�os
    FillConsoleOutputCharacter(m_hConsole, ' ', cellCount, m_homeCoords, &count);
    FillConsoleOutputAttribute(m_hConsole, csbi.wAttributes, cellCount, m_homeCoords, &count);

    // Reposiciona o cursor no in�cio
    SetConsoleCursorPosition(m_hConsole, m_homeCoords);
}

void WindowManager::cleanup() {

}
