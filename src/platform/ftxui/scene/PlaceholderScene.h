#pragma once

#include <functional>
#include <memory>
#include <string>

#include "FtxuiScene.h"

// Cena provisória da PoC para estados ainda não portados ao FTXUI
// (game/gameOver/record): mostra o título e volta ao menu com ESC.
class PlaceholderScene final : public FtxuiScene {
    std::string m_title;
    std::function<void()> m_goBack;

public:
    PlaceholderScene(std::string title, std::function<void()> goBack);

    void draw() override;
    void input() override;
};
