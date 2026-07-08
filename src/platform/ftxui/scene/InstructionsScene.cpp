#include "InstructionsScene.h"

#include <utility>

#include "8puzzle/game/GameRouter.h"
#include "../Keyboard.h"

using namespace ftxui;

InstructionsScene::InstructionsScene(std::shared_ptr<GameRouter> gameRouter)
    : m_gameRouter(std::move(gameRouter)) {}

void InstructionsScene::draw() {
    auto content = vbox({
        text("Como jogar") | bold | color(Color::Cyan) | hcenter,
        separator(),
        text(" "),
        text("• O tabuleiro 3x3 tem as peças de 1 a 8 e um espaço vazio."),
        text("• Use as SETAS para deslizar uma peça vizinha para o vazio."),
        text("• Vence quem ordenar as peças de 1 a 8, com o vazio no fim."),
        text("• Menos movimentos e menos tempo = melhor recorde."),
        text(" "),
    });

    present(vbox({
        filler(),
        window(text(" 8 · PUZZLE "), content) | size(WIDTH, EQUAL, 64) | hcenter,
        text(" "),
        text("Pressione ENTER para ir ao menu") | color(Color::Yellow) | hcenter,
        filler(),
        hints("ENTER menu"),
    }));
}

void InstructionsScene::input() {
    if (readKey() == Key::Enter) {
        m_gameRouter->menu();
    }
}
