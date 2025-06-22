//
// Created by mrmar on 22/06/2025.
//

#include "TerminalWindowManager.h"

#include <iostream>


using namespace std;

TerminalWindowManager::TerminalWindowManager() :
    m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
    m_homeCoords({0, 0}) {
}

void TerminalWindowManager::init() {

}

void TerminalWindowManager::update() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;

    // Obtém o tamanho do buffer do console
    if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi))
        return;

    const DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Preenche todo o console com espaços
    FillConsoleOutputCharacter(m_hConsole, ' ', cellCount, m_homeCoords, &count);
    FillConsoleOutputAttribute(m_hConsole, csbi.wAttributes, cellCount, m_homeCoords, &count);

    // Reposiciona o cursor no início
    SetConsoleCursorPosition(m_hConsole, m_homeCoords);
}

void TerminalWindowManager::cleanup() {

}