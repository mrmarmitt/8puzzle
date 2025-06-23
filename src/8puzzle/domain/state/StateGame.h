#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "../ApplicationGame.h"

class ApplicationGame;

class IStateGame {
public:

    virtual ~IStateGame() = default;

    [[nodiscard]] virtual StateEnum getGameState() const = 0;
    [[nodiscard]] virtual std::string getGameStateName() const = 0;
    virtual void introduction(ApplicationGame& game) = 0;
    virtual void menu(ApplicationGame& game) = 0;
    virtual void game(ApplicationGame& game) = 0;
    virtual void record(ApplicationGame& game) = 0;
    virtual void exit(ApplicationGame& game) = 0;
};


class InitialSG final : public IStateGame {
public:
    InitialSG() = default;
    [[nodiscard]] StateEnum getGameState() const override { return StateEnum::Initial; }
    [[nodiscard]] std::string getGameStateName() const override { return "Initial"; }
    void introduction(ApplicationGame& game) override; // Declaração, implementação abaixo
    void menu(ApplicationGame& game) override { std::cout << "It is not possible to select menu from initial state" << std::endl; }
    void game(ApplicationGame& game) override { std::cout << "It is not possible to select game from initial state" << std::endl; }
    void record(ApplicationGame& game) override { std::cout << "It is not possible to select record from initial state" << std::endl; }
    void exit(ApplicationGame& game) override { std::cout << "It is not possible to exit from initial state" << std::endl; }
};

class IntroductionSG final : public IStateGame {
public:
    IntroductionSG() = default;
    [[nodiscard]] StateEnum getGameState() const override { return StateEnum::Introduction; }
    [[nodiscard]] std::string getGameStateName() const override { return "Introduction"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from introduction state" << std::endl; }
    void menu(ApplicationGame& game) override; // Declaração, implementação abaixo
    void game(ApplicationGame& game) override { std::cout << "It is not possible to game from introduction state" << std::endl; }
    void record(ApplicationGame& game) override { std::cout << "It is not possible to select record from introduction state" << std::endl; }
    void exit(ApplicationGame& game) override { std::cout << "It is not possible to exit from introduction state" << std::endl; }
};

class MenuSG final : public IStateGame {
public:
    MenuSG() = default;
    [[nodiscard]] StateEnum getGameState() const override { return StateEnum::Menu; }
    [[nodiscard]] std::string getGameStateName() const override { return "Menu"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from menu state" << std::endl; }
    void menu(ApplicationGame& game) override { std::cout << "It is not possible to select menu from menu state" << std::endl; }
    void game(ApplicationGame& game) override; // Declaração, implementação abaixo
    void record(ApplicationGame& game) override; // Declaração, implementação abaixo
    void exit(ApplicationGame& game) override;  // Declaração, implementação abaixo
};

class GameSG final : public IStateGame {
public:
    GameSG() = default;
    [[nodiscard]] StateEnum getGameState() const override { return StateEnum::Game; }
    [[nodiscard]] std::string getGameStateName() const override { return "Game"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from game state" << std::endl; }
    void menu(ApplicationGame& game) override; // Declaração, implementação abaixo
    void game(ApplicationGame& game) override { std::cout << "It is not possible to select game from game state" << std::endl; }
    void record(ApplicationGame& game) override { std::cout << "It is not possible to select record from game state" << std::endl; }
    void exit(ApplicationGame& game) override { std::cout << "It is not possible to exit from game state" << std::endl; }
};

class RecordSG final : public IStateGame {
public:
    RecordSG() = default;
    [[nodiscard]] StateEnum getGameState() const override { return StateEnum::Record; }
    [[nodiscard]] std::string getGameStateName() const override { return "Record"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from record state" << std::endl; }
    void menu(ApplicationGame& game) override; // Declaração, implementação abaixo
    void game(ApplicationGame& game) override { std::cout << "It is not possible to select game from record state" << std::endl; }
    void record(ApplicationGame& game) override { std::cout << "It is not possible to select record from record state" << std::endl; }
    void exit(ApplicationGame& game) override { std::cout << "It is not possible to exit from record state" << std::endl; }
};

class ExitSG final : public IStateGame {
public:
    ExitSG() = default;
    [[nodiscard]] StateEnum getGameState() const override { return StateEnum::Exit; }
    [[nodiscard]] std::string getGameStateName() const override { return "Exit"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from exit state" << std::endl; }
    void menu(ApplicationGame& game) override { std::cout << "It is not possible to select menu from exit state" << std::endl; }
    void game(ApplicationGame& game) override { std::cout << "It is not possible to select game from exit state" << std::endl; }
    void record(ApplicationGame& game) override { std::cout << "It is not possible to select record from exit state" << std::endl; }
    void exit(ApplicationGame& game) override { std::cout << "Exiting application." << std::endl; }
};

inline void InitialSG::introduction(ApplicationGame& game) {
    game.setState(std::make_unique<IntroductionSG>());
}

inline void IntroductionSG::menu(ApplicationGame& game) {
    game.setState(std::make_unique<MenuSG>());
}

inline void MenuSG::game(ApplicationGame& game) {
    game.setState(std::make_unique<GameSG>());
}

inline void MenuSG::record(ApplicationGame& game) {
    game.setState(std::make_unique<RecordSG>());
}

inline void MenuSG::exit(ApplicationGame& game) {
    game.setState(std::make_unique<ExitSG>());
}

inline void GameSG::menu(ApplicationGame& game) {
    game.setState(std::make_unique<MenuSG>());
}

inline void RecordSG::menu(ApplicationGame& game) {
    game.setState(std::make_unique<MenuSG>());
}