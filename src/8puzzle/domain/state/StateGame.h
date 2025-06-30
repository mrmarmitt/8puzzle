#pragma once

#include <iostream>
#include <memory>
#include <string>

class GameRouter;

class IStateGame {
public:

    virtual ~IStateGame() = default;

    [[nodiscard]] virtual StateEnum getGameStateEnum() const = 0;
    [[nodiscard]] virtual std::string getGameStateName() const = 0;
    virtual void introduction(GameRouter& game) = 0;
    virtual void menu(GameRouter& game) = 0;
    virtual void game(GameRouter& game) = 0;
    virtual void gameOver(GameRouter& game) = 0;
    virtual void record(GameRouter& game) = 0;
    virtual void exit(GameRouter& game) = 0;
};


class InitialSG final : public IStateGame {
public:
    InitialSG() = default;
    [[nodiscard]] StateEnum getGameStateEnum() const override { return StateEnum::Initial; }
    [[nodiscard]] std::string getGameStateName() const override { return "Initial"; }
    void introduction(GameRouter& game) override; // Declaração, implementação abaixo
    void menu(GameRouter& game) override { std::cout << "It is not possible to select menu from initial state" << std::endl; }
    void game(GameRouter& game) override { std::cout << "It is not possible to select game from initial state" << std::endl; }
    void gameOver(GameRouter& game) override { std::cout << "It is not possible to select game over from initial state" << std::endl; }
    void record(GameRouter& game) override { std::cout << "It is not possible to select record from initial state" << std::endl; }
    void exit(GameRouter& game) override { std::cout << "It is not possible to exit from initial state" << std::endl; }
};

class IntroductionSG final : public IStateGame {
public:
    IntroductionSG() = default;
    [[nodiscard]] StateEnum getGameStateEnum() const override { return StateEnum::Introduction; }
    [[nodiscard]] std::string getGameStateName() const override { return "Introduction"; }
    void introduction(GameRouter& game) override { std::cout << "It is not possible to select introduction from introduction state" << std::endl; }
    void menu(GameRouter& game) override; // Declaração, implementação abaixo
    void game(GameRouter& game) override { std::cout << "It is not possible to game from introduction state" << std::endl; }
    void gameOver(GameRouter& game) override { std::cout << "It is not possible to select game over from introduction state" << std::endl; }
    void record(GameRouter& game) override { std::cout << "It is not possible to select record from introduction state" << std::endl; }
    void exit(GameRouter& game) override { std::cout << "It is not possible to exit from introduction state" << std::endl; }
};

class MenuSG final : public IStateGame {
public:
    MenuSG() = default;
    [[nodiscard]] StateEnum getGameStateEnum() const override { return StateEnum::Menu; }
    [[nodiscard]] std::string getGameStateName() const override { return "Menu"; }
    void introduction(GameRouter& game) override { std::cout << "It is not possible to select introduction from menu state" << std::endl; }
    void menu(GameRouter& game) override { std::cout << "It is not possible to select menu from menu state" << std::endl; }
    void game(GameRouter& game) override; // Declaração, implementação abaixo
    void gameOver(GameRouter& game) override { std::cout << "It is not possible to select game over from menu state" << std::endl; }
    void record(GameRouter& game) override; // Declaração, implementação abaixo
    void exit(GameRouter& game) override;  // Declaração, implementação abaixo
};

class GameSG final : public IStateGame {
public:
    GameSG() = default;
    [[nodiscard]] StateEnum getGameStateEnum() const override { return StateEnum::Game; }
    [[nodiscard]] std::string getGameStateName() const override { return "Game"; }
    void introduction(GameRouter& game) override { std::cout << "It is not possible to select introduction from game state" << std::endl; }
    void menu(GameRouter& game) override; // Declaração, implementação abaixo
    void game(GameRouter& game) override { std::cout << "It is not possible to select game from game state" << std::endl; }
    void gameOver(GameRouter& game) override; // Declaração, implementação abaixo
    void record(GameRouter& game) override { std::cout << "It is not possible to select record from game state" << std::endl; }
    void exit(GameRouter& game) override { std::cout << "It is not possible to exit from game state" << std::endl; }
};

class GameOverSG final : public IStateGame {
public:
    GameOverSG() = default;
    [[nodiscard]] StateEnum getGameStateEnum() const override { return StateEnum::GameOver; }
    [[nodiscard]] std::string getGameStateName() const override { return "GameOver"; }
    void introduction(GameRouter& game) override { std::cout << "It is not possible to select introduction from game over state" << std::endl; }
    void menu(GameRouter& game) override; // Declaração, implementação abaixo
    void game(GameRouter& game) override { std::cout << "It is not possible to select game from game over state" << std::endl; }
    void gameOver(GameRouter& game) override { std::cout << "It is not possible to select game over over from game over state" << std::endl; }
    void record(GameRouter& game) override { std::cout << "It is not possible to select record from game over state" << std::endl; }
    void exit(GameRouter& game) override { std::cout << "It is not possible to exit from game over state" << std::endl; }
};

class RecordSG final : public IStateGame {
public:
    RecordSG() = default;
    [[nodiscard]] StateEnum getGameStateEnum() const override { return StateEnum::Record; }
    [[nodiscard]] std::string getGameStateName() const override { return "Record"; }
    void introduction(GameRouter& game) override { std::cout << "It is not possible to select introduction from record state" << std::endl; }
    void menu(GameRouter& game) override; // Declaração, implementação abaixo
    void game(GameRouter& game) override { std::cout << "It is not possible to select game from record state" << std::endl; }
    void gameOver(GameRouter& game) override { std::cout << "It is not possible to select game over over from record state" << std::endl; }
    void record(GameRouter& game) override { std::cout << "It is not possible to select record from record state" << std::endl; }
    void exit(GameRouter& game) override { std::cout << "It is not possible to exit from record state" << std::endl; }
};

class ExitSG final : public IStateGame {
public:
    ExitSG() = default;
    [[nodiscard]] StateEnum getGameStateEnum() const override { return StateEnum::Exit; }
    [[nodiscard]] std::string getGameStateName() const override { return "Exit"; }
    void introduction(GameRouter& game) override { std::cout << "It is not possible to select introduction from exit state" << std::endl; }
    void menu(GameRouter& game) override { std::cout << "It is not possible to select menu from exit state" << std::endl; }
    void game(GameRouter& game) override { std::cout << "It is not possible to select game from exit state" << std::endl; }
    void gameOver(GameRouter& game) override { std::cout << "It is not possible to select game over over from exit state" << std::endl; }
    void record(GameRouter& game) override { std::cout << "It is not possible to select record from exit state" << std::endl; }
    void exit(GameRouter& game) override { std::cout << "Exiting application." << std::endl; }
};

inline void InitialSG::introduction(GameRouter& game) {
    game.setState(std::make_unique<IntroductionSG>());
}

inline void IntroductionSG::menu(GameRouter& game) {
    game.setState(std::make_unique<MenuSG>());
}

inline void MenuSG::game(GameRouter& game) {
    game.setState(std::make_unique<GameSG>());
}

inline void MenuSG::record(GameRouter& game) {
    game.setState(std::make_unique<RecordSG>());
}

inline void MenuSG::exit(GameRouter& game) {
    game.setState(std::make_unique<ExitSG>());
}

inline void GameSG::menu(GameRouter& game) {
    game.setState(std::make_unique<MenuSG>());
}

inline void GameSG::gameOver(GameRouter& game) {
    game.setState(std::make_unique<GameOverSG>());
}

inline void GameOverSG::menu(GameRouter& game) {
    game.setState(std::make_unique<MenuSG>());
}

inline void RecordSG::menu(GameRouter& game) {
    game.setState(std::make_unique<MenuSG>());
}