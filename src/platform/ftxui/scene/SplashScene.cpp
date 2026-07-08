#include "SplashScene.h"

#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "8puzzle/game/GameRouter.h"
#include "../Keyboard.h"

using namespace ftxui;

namespace {

Element tile(const std::string& label) {
    const bool empty = label.empty();
    Element cell = text(label) | bold | center
        | size(WIDTH, EQUAL, 7) | size(HEIGHT, EQUAL, 3);
    if (!empty) {
        cell = cell | color(Color::Cyan);
    }
    return cell | borderRounded;
}

// Tabuleiro resolvido, decorativo, provando o gridbox que a GameScene usará.
Element decorativeBoard() {
    return gridbox({
        {tile("1"), tile("2"), tile("3")},
        {tile("4"), tile("5"), tile("6")},
        {tile("7"), tile("8"), tile("")},
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
    switch (readKey()) {
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
