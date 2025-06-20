#pragma once
#include <memory>

#include "IStateGame.h"

class ApplicationGame final {
    std::unique_ptr<IStateGame> m_state;

public:
    ApplicationGame();
    ~ApplicationGame() = default;

    void setState(std::unique_ptr<IStateGame> state);

    [[nodiscard]] IStateGame& getState() const;
    [[nodiscard]] std::string getGameStateName() const;
    [[nodiscard]] GameState getGameState() const;

    void introduction();
    void menu();
    void exit();
};

