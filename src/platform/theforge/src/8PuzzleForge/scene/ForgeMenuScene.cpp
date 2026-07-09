#include "ForgeMenuScene.h"

#include <string>
#include <utility>
#include <vector>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"

namespace {
const std::vector<std::string> kOptions = {"Novo Jogo", "Recorde", "Configuracoes", "Sair"};
} // namespace

ForgeMenuScene::ForgeMenuScene(std::shared_ptr<GameRouter> gameRouter,
                               std::shared_ptr<GamePlayService> gamePlayService)
    : m_gameRouter(std::move(gameRouter)),
      m_gamePlayService(std::move(gamePlayService)) {}

void ForgeMenuScene::draw() {
    const float h = forgeui::screenHeight();

    forgeui::drawTextCentered("8 . P U Z Z L E", h * 0.16f, 48.0f, forgeui::color::kTitle);
    forgeui::drawTextCentered("MENU PRINCIPAL", h * 0.16f + 72.0f, 22.0f, forgeui::color::kDim);

    float y = h * 0.42f;
    for (int i = 0; i < static_cast<int>(kOptions.size()); ++i) {
        const bool selected = (i == m_selected);
        const std::string label = (selected ? "> " : "  ") + kOptions[i];
        forgeui::drawTextCentered(label, y, selected ? 30.0f : 26.0f,
                                  selected ? forgeui::color::kAccent : forgeui::color::kText);
        y += 52.0f;
    }

    forgeui::drawHints("SETAS navegar   ENTER selecionar   ESC sair");
}

void ForgeMenuScene::input() {
    const int numOptions = static_cast<int>(kOptions.size());

    switch (forgeui::readKey().key) {
        case Key::Up:
            m_selected = (m_selected == 0) ? (numOptions - 1) : (m_selected - 1);
            break;
        case Key::Down:
            m_selected = (m_selected == numOptions - 1) ? 0 : (m_selected + 1);
            break;
        case Key::Enter:
            switch (m_selected) {
                case 0:
                    m_gamePlayService->startNewGame();
                    m_gameRouter->game();
                    break;
                case 1:
                    m_gameRouter->record();
                    break;
                case 2:
                    // Configuracoes ainda nao e um estado do jogo (como nas
                    // demais plataformas).
                    break;
                case 3:
                    m_gameRouter->exit();
                    break;
                default: ;
            }
            break;
        case Key::Escape:
            m_gameRouter->exit();
            break;
        default:
            break;
    }
}
