#include "FtxuiWindowManager.h"

#include <iostream>
#include <thread>

#include <windows.h>

void FtxuiWindowManager::init() {
    // Habilita sequências ANSI (cores/cursor do FTXUI) e UTF-8 no console.
    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(console, &mode)) {
        SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    SetConsoleOutputCP(CP_UTF8);

    // Limpa a tela e esconde o cursor durante o jogo.
    std::cout << "\x1b[2J\x1b[?25l" << std::flush;

    m_nextFrame = Clock::now() + kFramePeriod;
}

void FtxuiWindowManager::update() {
    // Sem input bloqueante, o loop giraria a 100% de CPU — o teto de quadros
    // vive aqui, na plataforma (a cengine entrega o tempo via update(dt)).
    std::this_thread::sleep_until(m_nextFrame);
    m_nextFrame = Clock::now() + kFramePeriod;

    // Cursor no topo: a cena reescreve o quadro inteiro por cima do anterior.
    std::cout << "\x1b[H";
}

void FtxuiWindowManager::cleanup() {
    // Restaura o cursor e os atributos do console.
    std::cout << "\x1b[0m\x1b[?25h" << std::endl;
}
