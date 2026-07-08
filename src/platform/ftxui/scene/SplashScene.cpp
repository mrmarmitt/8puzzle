#include "SplashScene.h"

#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "8puzzle/game/GameRouter.h"
#include "../BoardView.h"
#include "../Keyboard.h"

using namespace ftxui;

namespace {

// Tabuleiro resolvido, decorativo (mesmo visual da FtxuiGameScene).
Element decorativeBoard() {
    return gridbox({
        {tileCell("1"), tileCell("2"), tileCell("3")},
        {tileCell("4"), tileCell("5"), tileCell("6")},
        {tileCell("7"), tileCell("8"), tileCell("")},
    });
}

} // namespace

SplashScene::SplashScene(std::shared_ptr<GameRouter> gameRouter)
    : m_gameRouter(std::move(gameRouter)) {}

void SplashScene::update(const cengine::core::Seconds dt) {
    m_elapsed += dt;
}

void SplashScene::draw() {
    // Piscar suave dirigido pelo tempo de simulação (dt), não por frames.
    const bool promptVisible = std::fmod(m_elapsed.count(), 1.2) < 0.8;

    auto prompt = promptVisible
        ? text("Pressione ENTER para começar") | bold | color(Color::Yellow)
        : text(" ");

    present(vbox({
        filler(),
        text("8 · P U Z Z L E") | bold | color(Color::Cyan) | hcenter,
        text(" "),
        decorativeBoard() | hcenter,
        text(" "),
        prompt | hcenter,
        filler(),
        hints("ENTER começar   ESC sair"),
    }));
}

void SplashScene::input() {
    switch (readKey().key) {
        case Key::Enter:
            m_gameRouter->introduction();
            break;
        case Key::Escape:
            m_gameRouter->introduction(); // sair só existe a partir do menu
            break;
        default:
            break;
    }
}
