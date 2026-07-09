#include "ForgeInstructionsScene.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "8puzzle/game/GameRouter.h"

ForgeInstructionsScene::ForgeInstructionsScene(std::shared_ptr<GameRouter> gameRouter)
    : m_gameRouter(std::move(gameRouter)) {}

void ForgeInstructionsScene::draw() {
    const float h = forgeui::screenHeight();
    constexpr float kLineSize = 22.0f;

    forgeui::drawTextCentered("Como jogar", h * 0.18f, 40.0f, forgeui::color::kTitle);

    const std::vector<std::string> lines = {
        "- O tabuleiro 3x3 tem as pecas de 1 a 8 e um espaco vazio.",
        "- Use as SETAS para deslizar uma peca vizinha para o vazio",
        "  (ou digite o NUMERO da peca, de 1 a 8).",
        "- Vence quem ordenar as pecas de 1 a 8, com o vazio no fim.",
        "- Menos movimentos e menos tempo = melhor recorde.",
    };

    // Bloco alinhado a esquerda, centrado como um todo.
    float maxWidth = 0.0f;
    for (const auto& line : lines) {
        maxWidth = std::max(maxWidth, forgeui::textWidth(line, kLineSize));
    }
    const float x = (forgeui::screenWidth() - maxWidth) * 0.5f;

    float y = h * 0.32f;
    for (const auto& line : lines) {
        forgeui::drawText(line, x, y, kLineSize, forgeui::color::kText);
        y += 34.0f;
    }

    forgeui::drawTextCentered("Pressione ENTER para ir ao menu", y + 30.0f, 24.0f, forgeui::color::kValue);

    forgeui::drawHints("ENTER menu");
}

void ForgeInstructionsScene::input() {
    if (forgeui::readKey().key == Key::Enter) {
        m_gameRouter->menu();
    }
}
