#pragma once

#include <iostream>

#include "8puzzle/engine/IRouter.h"
#include "StateGameFlow.h"

class InitialSG final : public StateGameFlow {
public:
    InitialSG() = default;

    [[nodiscard]] std::string getCode() const override { return "initial"; }
    [[nodiscard]] std::string getName() const override { return "Initial"; }
    [[nodiscard]] std::unique_ptr<IState> clone() const override { return std::make_unique<InitialSG>(*this); }

    void introduction(IRouter& game) override;// Declaração, implementação abaixo
    void menu(IRouter& game) override { std::cout << "It is not possible to select menu from initial state" << std::endl; }
    void game(IRouter& game) override { std::cout << "It is not possible to select game from initial state" << std::endl; }
    void gameOver(IRouter& game) override { std::cout << "It is not possible to select game over from initial state" << std::endl; }
    void record(IRouter& game) override { std::cout << "It is not possible to select record from initial state" << std::endl; }
    void exit(IRouter& game) override { std::cout << "It is not possible to exit from initial state" << std::endl; }
};

class IntroductionSG final : public StateGameFlow {
public:
    IntroductionSG() = default;

    [[nodiscard]] std::string getCode() const override { return "introduction"; }
    [[nodiscard]] std::string getName() const override { return "Introduction"; }
    [[nodiscard]] std::unique_ptr<IState> clone() const override { return std::make_unique<IntroductionSG>(*this); }

    void introduction(IRouter& game) override { std::cout << "It is not possible to select introduction from introduction state" << std::endl; }
    void menu(IRouter& game) override; // Declaração, implementação abaixo
    void game(IRouter& game) override { std::cout << "It is not possible to game from introduction state" << std::endl; }
    void gameOver(IRouter& game) override { std::cout << "It is not possible to select game over from introduction state" << std::endl; }
    void record(IRouter& game) override { std::cout << "It is not possible to select record from introduction state" << std::endl; }
    void exit(IRouter& game) override { std::cout << "It is not possible to exit from introduction state" << std::endl; }
};

class MenuSG final : public StateGameFlow {
public:
    MenuSG() = default;

    [[nodiscard]] std::string getCode() const override { return "menu"; }
    [[nodiscard]] std::string getName() const override { return "Menu"; }
    [[nodiscard]] std::unique_ptr<IState> clone() const override { return std::make_unique<MenuSG>(*this); }

    void introduction(IRouter& game) override { std::cout << "It is not possible to select introduction from menu state" << std::endl; }
    void menu(IRouter& game) override { std::cout << "It is not possible to select menu from menu state" << std::endl; }
    void game(IRouter& game) override; // Declaração, implementação abaixo
    void gameOver(IRouter& game) override { std::cout << "It is not possible to select game over from menu state" << std::endl; }
    void record(IRouter& game) override; // Declaração, implementação abaixo
    void exit(IRouter& game) override;  // Declaração, implementação abaixo
};

class GameSG final : public StateGameFlow {
public:
    GameSG() = default;

    [[nodiscard]] std::string getCode() const override { return "game"; }
    [[nodiscard]] std::string getName() const override { return "Game"; }
    [[nodiscard]] std::unique_ptr<IState> clone() const override { return std::make_unique<GameSG>(*this); }

    void introduction(IRouter& game) override { std::cout << "It is not possible to select introduction from game state" << std::endl; }
    void menu(IRouter& game) override; // Declaração, implementação abaixo
    void game(IRouter& game) override { std::cout << "It is not possible to select game from game state" << std::endl; }
    void gameOver(IRouter& game) override; // Declaração, implementação abaixo
    void record(IRouter& game) override { std::cout << "It is not possible to select record from game state" << std::endl; }
    void exit(IRouter& game) override { std::cout << "It is not possible to exit from game state" << std::endl; }
};

class GameOverSG final : public StateGameFlow {
public:
    GameOverSG() = default;

    [[nodiscard]] std::string getCode() const override { return "gameOver"; }
    [[nodiscard]] std::string getName() const override { return "GameOver"; }
    [[nodiscard]] std::unique_ptr<IState> clone() const override { return std::make_unique<GameOverSG>(*this); }

    void introduction(IRouter& game) override { std::cout << "It is not possible to select introduction from game over state" << std::endl; }
    void menu(IRouter& game) override; // Declaração, implementação abaixo
    void game(IRouter& game) override { std::cout << "It is not possible to select game from game over state" << std::endl; }
    void gameOver(IRouter& game) override { std::cout << "It is not possible to select game over over from game over state" << std::endl; }
    void record(IRouter& game) override { std::cout << "It is not possible to select record from game over state" << std::endl; }
    void exit(IRouter& game) override { std::cout << "It is not possible to exit from game over state" << std::endl; }
};

class RecordSG final : public StateGameFlow {
public:
    RecordSG() = default;

    [[nodiscard]] std::string getCode() const override { return "record"; }
    [[nodiscard]] std::string getName() const override { return "Record"; }
    [[nodiscard]] std::unique_ptr<IState> clone() const override { return std::make_unique<RecordSG>(*this); }

    void introduction(IRouter& game) override { std::cout << "It is not possible to select introduction from record state" << std::endl; }
    void menu(IRouter& game) override; // Declaração, implementação abaixo
    void game(IRouter& game) override { std::cout << "It is not possible to select game from record state" << std::endl; }
    void gameOver(IRouter& game) override { std::cout << "It is not possible to select game over over from record state" << std::endl; }
    void record(IRouter& game) override { std::cout << "It is not possible to select record from record state" << std::endl; }
    void exit(IRouter& game) override { std::cout << "It is not possible to exit from record state" << std::endl; }
};

class ExitSG final : public StateGameFlow {
public:
    ExitSG() = default;

    [[nodiscard]] std::string getCode() const override { return "exit"; }
    [[nodiscard]] std::string getName() const override { return "Exit"; }
    [[nodiscard]] std::unique_ptr<IState> clone() const override { return std::make_unique<ExitSG>(*this); }

    void introduction(IRouter& game) override { std::cout << "It is not possible to select introduction from exit state" << std::endl; }
    void menu(IRouter& game) override { std::cout << "It is not possible to select menu from exit state" << std::endl; }
    void game(IRouter& game) override { std::cout << "It is not possible to select game from exit state" << std::endl; }
    void gameOver(IRouter& game) override { std::cout << "It is not possible to select game over over from exit state" << std::endl; }
    void record(IRouter& game) override { std::cout << "It is not possible to select record from exit state" << std::endl; }
    void exit(IRouter& game) override { std::cout << "Exiting application." << std::endl; }
};

inline void InitialSG::introduction(IRouter& game) {
    game.setNextState(std::make_unique<IntroductionSG>());
}

inline void IntroductionSG::menu(IRouter& game) {
    game.setNextState(std::make_unique<MenuSG>());
}

inline void MenuSG::game(IRouter& game) {
    game.setNextState(std::make_unique<GameSG>());
}

inline void MenuSG::record(IRouter& game) {
    game.setNextState(std::make_unique<RecordSG>());
}

inline void MenuSG::exit(IRouter& game) {
    game.setNextState(std::make_unique<ExitSG>());
}

inline void GameSG::menu(IRouter& game) {
    game.setNextState(std::make_unique<MenuSG>());
}

inline void GameSG::gameOver(IRouter& game) {
    game.setNextState(std::make_unique<GameOverSG>());
}

inline void GameOverSG::menu(IRouter& game) {
    game.setNextState(std::make_unique<MenuSG>());
}

inline void RecordSG::menu(IRouter& game) {
    game.setNextState(std::make_unique<MenuSG>());
}