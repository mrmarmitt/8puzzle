#include "ForgeSplashScene.h"

#include <cmath>
#include <utility>

#include "8puzzle/game/GameRouter.h"
#include "../ForgeBoardView.h"

ForgeSplashScene::ForgeSplashScene(std::shared_ptr<GameRouter> gameRouter)
    : m_gameRouter(std::move(gameRouter)) {}

void ForgeSplashScene::update(const cengine::core::Seconds dt) {
    m_elapsed += dt;
}

void ForgeSplashScene::draw() {
    const float h = forgeui::screenHeight();

    forgeui::drawTextCentered("8 . P U Z Z L E", h * 0.14f, 56.0f, forgeui::color::kTitle);

    // Tabuleiro resolvido, decorativo.
    drawTileGrid({"1", "2", "3", "4", "5", "6", "7", "8", ""},
                 forgeui::screenWidth() * 0.5f, h * 0.28f, 88.0f);

    // Piscar suave dirigido pelo tempo de simulacao (dt), nao por frames.
    if (std::fmod(m_elapsed.count(), 1.2) < 0.8) {
        forgeui::drawTextCentered("Pressione ENTER para comecar", h * 0.28f + boardHeight(88.0f) + 40.0f,
                                  24.0f, forgeui::color::kValue);
    }

    forgeui::drawHints("ENTER comecar   ESC sair");
}

void ForgeSplashScene::input() {
    switch (forgeui::readKey().key) {
        case Key::Enter:
            m_gameRouter->introduction();
            break;
        case Key::Escape:
            m_gameRouter->introduction(); // sair so existe a partir do menu
            break;
        default:
            break;
    }
}
