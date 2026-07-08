#include "FtxuiMenuScene.h"

#include <string>
#include <utility>
#include <vector>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "../Keyboard.h"

using namespace ftxui;

namespace {
const std::vector<std::string> kOptions = {"Novo Jogo", "Recorde", "Configurações", "Sair"};
} // namespace

FtxuiMenuScene::FtxuiMenuScene(std::shared_ptr<GameRouter> gameRouter,
                               std::shared_ptr<GamePlayService> gamePlayService)
    : m_gameRouter(std::move(gameRouter)),
      m_gamePlayService(std::move(gamePlayService)) {}

void FtxuiMenuScene::draw() {
    Elements items;
    for (int i = 0; i < static_cast<int>(kOptions.size()); ++i) {
        const bool selected = (i == m_selected);
        Element item = text((selected ? "  ▶  " : "     ") + kOptions[i] + "  ");
        if (selected) {
            item = item | bold | color(Color::Black) | bgcolor(Color::Cyan);
        }
        items.push_back(item);
        items.push_back(text(" "));
    }

    auto menu = vbox({
        text("MENU PRINCIPAL") | bold | hcenter,
        separator(),
        text(" "),
        vbox(std::move(items)) | hcenter,
    });

    present(vbox({
        filler(),
        window(text(" 8 · PUZZLE "), menu) | size(WIDTH, EQUAL, 44) | hcenter,
        filler(),
        hints("↑/↓ navegar   ENTER selecionar   ESC sair"),
    }));
}

void FtxuiMenuScene::input() {
    constexpr int numOptions = 4;

    switch (readKey()) {
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
                    // Configurações ainda não é um estado do jogo (como no terminal).
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
