#pragma once

#include <iostream>

#include "8puzzle/game/GameRouter.h"
#include "StateGameFlow.h"

class InitialSG final : public StateGameFlow {
public:
    InitialSG() = default;

    [[nodiscard]] std::string getCode() const override { return "initial"; }
    [[nodiscard]] std::string getName() const override { return "Initial"; }

    void introduction(GameRouter& game) const override;// Declaração, implementação abaixo
    void menu(GameRouter& game) const override { std::cout << "It is not possible to select menu from initial state" << std::endl; }
    void game(GameRouter& game) const override { std::cout << "It is not possible to select game from initial state" << std::endl; }
    void gameOver(GameRouter& game) const override { std::cout << "It is not possible to select game over from initial state" << std::endl; }
    void record(GameRouter& game) const override { std::cout << "It is not possible to select record from initial state" << std::endl; }
    void exit(GameRouter& game) const override { std::cout << "It is not possible to exit from initial state" << std::endl; }
};

class IntroductionSG final : public StateGameFlow {
public:
    IntroductionSG() = default;

    [[nodiscard]] std::string getCode() const override { return "introduction"; }
    [[nodiscard]] std::string getName() const override { return "Introduction"; }

    void introduction(GameRouter& game) const override { std::cout << "It is not possible to select introduction from introduction state" << std::endl; }
    void menu(GameRouter& game) const override; // Declaração, implementação abaixo
    void game(GameRouter& game) const override { std::cout << "It is not possible to game from introduction state" << std::endl; }
    void gameOver(GameRouter& game) const override { std::cout << "It is not possible to select game over from introduction state" << std::endl; }
    void record(GameRouter& game) const override { std::cout << "It is not possible to select record from introduction state" << std::endl; }
    void exit(GameRouter& game) const override { std::cout << "It is not possible to exit from introduction state" << std::endl; }
};

class MenuSG final : public StateGameFlow {
public:
    MenuSG() = default;

    [[nodiscard]] std::string getCode() const override { return "menu"; }
    [[nodiscard]] std::string getName() const override { return "Menu"; }

    void introduction(GameRouter& game) const override { std::cout << "It is not possible to select introduction from menu state" << std::endl; }
    void menu(GameRouter& game) const override { std::cout << "It is not possible to select menu from menu state" << std::endl; }
    void game(GameRouter& game) const override; // Declaração, implementação abaixo
    void gameOver(GameRouter& game) const override { std::cout << "It is not possible to select game over from menu state" << std::endl; }
    void record(GameRouter& game) const override; // Declaração, implementação abaixo
    void exit(GameRouter& game) const override;  // Declaração, implementação abaixo
};

class GameSG final : public StateGameFlow {
public:
    GameSG() = default;

    [[nodiscard]] std::string getCode() const override { return "game"; }
    [[nodiscard]] std::string getName() const override { return "Game"; }

    void introduction(GameRouter& game) const override { std::cout << "It is not possible to select introduction from game state" << std::endl; }
    void menu(GameRouter& game) const override; // Declaração, implementação abaixo
    void game(GameRouter& game) const override { std::cout << "It is not possible to select game from game state" << std::endl; }
    void gameOver(GameRouter& game) const override; // Declaração, implementação abaixo
    void record(GameRouter& game) const override { std::cout << "It is not possible to select record from game state" << std::endl; }
    void exit(GameRouter& game) const override { std::cout << "It is not possible to exit from game state" << std::endl; }
};

class GameOverSG final : public StateGameFlow {
public:
    GameOverSG() = default;

    [[nodiscard]] std::string getCode() const override { return "gameOver"; }
    [[nodiscard]] std::string getName() const override { return "GameOver"; }

    void introduction(GameRouter& game) const override { std::cout << "It is not possible to select introduction from game over state" << std::endl; }
    void menu(GameRouter& game) const override; // Declaração, implementação abaixo
    void game(GameRouter& game) const override { std::cout << "It is not possible to select game from game over state" << std::endl; }
    void gameOver(GameRouter& game) const override { std::cout << "It is not possible to select game over over from game over state" << std::endl; }
    void record(GameRouter& game) const override { std::cout << "It is not possible to select record from game over state" << std::endl; }
    void exit(GameRouter& game) const override { std::cout << "It is not possible to exit from game over state" << std::endl; }
};

class RecordSG final : public StateGameFlow {
public:
    RecordSG() = default;

    [[nodiscard]] std::string getCode() const override { return "record"; }
    [[nodiscard]] std::string getName() const override { return "Record"; }

    void introduction(GameRouter& game) const override { std::cout << "It is not possible to select introduction from record state" << std::endl; }
    void menu(GameRouter& game) const override; // Declaração, implementação abaixo
    void game(GameRouter& game) const override { std::cout << "It is not possible to select game from record state" << std::endl; }
    void gameOver(GameRouter& game) const override { std::cout << "It is not possible to select game over over from record state" << std::endl; }
    void record(GameRouter& game) const override { std::cout << "It is not possible to select record from record state" << std::endl; }
    void exit(GameRouter& game) const override { std::cout << "It is not possible to exit from record state" << std::endl; }
};

class ExitSG final : public StateGameFlow {
public:
    ExitSG() = default;

    [[nodiscard]] std::string getCode() const override { return "exit"; }
    [[nodiscard]] std::string getName() const override { return "Exit"; }

    void introduction(GameRouter& game) const override { std::cout << "It is not possible to select introduction from exit state" << std::endl; }
    void menu(GameRouter& game) const override { std::cout << "It is not possible to select menu from exit state" << std::endl; }
    void game(GameRouter& game) const override { std::cout << "It is not possible to select game from exit state" << std::endl; }
    void gameOver(GameRouter& game) const override { std::cout << "It is not possible to select game over over from exit state" << std::endl; }
    void record(GameRouter& game) const override { std::cout << "It is not possible to select record from exit state" << std::endl; }
    void exit(GameRouter& game) const override { std::cout << "Exiting application." << std::endl; }
};

inline void InitialSG::introduction(GameRouter& game) const {
    game.setNextState(std::make_unique<IntroductionSG>());
}

inline void IntroductionSG::menu(GameRouter& game) const {
    game.setNextState(std::make_unique<MenuSG>());
}

inline void MenuSG::game(GameRouter& game) const {
    game.setNextState(std::make_unique<GameSG>());
}

inline void MenuSG::record(GameRouter& game) const {
    game.setNextState(std::make_unique<RecordSG>());
}

inline void MenuSG::exit(GameRouter& game) const {
    game.setNextState(std::make_unique<ExitSG>());
}

inline void GameSG::menu(GameRouter& game) const {
    game.setNextState(std::make_unique<MenuSG>());
}

inline void GameSG::gameOver(GameRouter& game) const {
    game.setNextState(std::make_unique<GameOverSG>());
}

inline void GameOverSG::menu(GameRouter& game) const {
    game.setNextState(std::make_unique<MenuSG>());
}

inline void RecordSG::menu(GameRouter& game) const {
    game.setNextState(std::make_unique<MenuSG>());
}
