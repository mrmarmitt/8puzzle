#include "IntroductionScreen.h"

#include <conio.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "8puzzle/game/GameRouter.h"

void IntroductionScreen::draw() {
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
    getGameRouter().menu();
}

void IntroductionScreen::input() {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == '\r') {
            getGameRouter().menu();
        }
    }
}