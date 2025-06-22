#include "IntroductionScreen.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>


void IntroductionScreen::draw(ApplicationGame& applicationGame) {
    const std::string title = R"(
      ____       ____  _   _ _     _
     |  _ \ ___ |  _ \| | | (_)___| |__
     | |_) / _ \| |_) | |_| | / __| '_ \
     |  __/ (_) |  __/|  _  | \__ \ | | |
     |_|   \___/|_|   |_| |_|_|___/_| |_|)";

        std::cout << "\n\n";

        // Animação: imprime uma letra por vez
        for (char ch : title) {
            std::cout << ch << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(2)); // controle da velocidade
        }
    std::cout << std::endl;
    applicationGame.menu();
}

void IntroductionScreen::input(ApplicationGame& applicationGame) {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == '\r') {
            applicationGame.menu();
        }
    }
}