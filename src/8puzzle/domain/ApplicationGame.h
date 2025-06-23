#pragma once
#include <memory>

#include "state/StateEnum.h"

class IStateGame;

class ApplicationGame final {
    std::unique_ptr<IStateGame> m_state;

public:
    ApplicationGame();
    ~ApplicationGame();

    void setState(std::unique_ptr<IStateGame> state);

    [[nodiscard]] IStateGame& getState() const;
    [[nodiscard]] std::string getGameStateName() const;
    [[nodiscard]] StateEnum getGameState() const;

    void introduction();
    void menu();
    void game();
    void record();
    void exit();
};

