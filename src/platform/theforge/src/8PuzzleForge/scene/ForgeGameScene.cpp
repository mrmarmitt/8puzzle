#include "ForgeGameScene.h"

#include <chrono>
#include <string>
#include <utility>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "platform/Format.h"
#include "../ForgeBoardView.h"

ForgeGameScene::ForgeGameScene(std::shared_ptr<GameRouter> gameRouter,
                               std::shared_ptr<GamePlayService> gamePlayService)
    : m_gameRouter(std::move(gameRouter)),
      m_gamePlayService(std::move(gamePlayService)) {}

void ForgeGameScene::draw() {
    const auto& gamePlay = m_gamePlayService->getGamePlay();
    const float h = forgeui::screenHeight();
    constexpr float kCellSize = 96.0f;

    // O tempo exibido e o mesmo que vale para o recorde: relogio de parede
    // desde o inicio da partida (dt e tempo de simulacao; nao serve aqui).
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - gamePlay.getStartedAt()).count();

    forgeui::drawTextCentered("8 . PUZZLE - jogo", h * 0.08f, 32.0f, forgeui::color::kTitle);

    const float boardTop = h * 0.20f;
    drawBoard(m_board, forgeui::screenWidth() * 0.5f, boardTop, kCellSize);

    const std::string stats = "Movimentos: " + std::to_string(gamePlay.getNumberOfMoves())
        + "     Tempo: " + formatMillis(elapsed);
    forgeui::drawTextCentered(stats, boardTop + boardHeight(kCellSize) + 48.0f, 24.0f, forgeui::color::kValue);

    forgeui::drawHints("SETAS deslizar peca   1-8 mover pelo numero   ESC menu");
}

int ForgeGameScene::neighborOfEmpty(const int dLine, const int dColumn) const {
    const auto [emptyLine, emptyColumn] = m_board.getEmptyPosition();
    const int line = emptyLine + dLine;
    const int column = emptyColumn + dColumn;

    if (line < 0 || line > 2 || column < 0 || column > 2) {
        return -1;
    }
    return m_board.getBoard()[line * 3 + column].getId();
}

void ForgeGameScene::tryMove(const int tileId) {
    if (tileId <= 0) {
        return;
    }

    auto& gamePlay = m_gamePlayService->getGamePlay();
    if (!m_board.moveTile(tileId)) {
        return;
    }
    gamePlay.incrementMove();

    if (m_board.isSolved()) {
        gamePlay.completeGame();
        m_gameRouter->gameOver();
    }
}

void ForgeGameScene::input() {
    const KeyEvent event = forgeui::readKey();

    switch (event.key) {
        // A seta indica a direcao em que a PECA desliza: seta para cima move
        // a peca que esta ABAIXO do vazio, e assim por diante.
        case Key::Up:    tryMove(neighborOfEmpty(+1, 0)); break;
        case Key::Down:  tryMove(neighborOfEmpty(-1, 0)); break;
        case Key::Left:  tryMove(neighborOfEmpty(0, +1)); break;
        case Key::Right: tryMove(neighborOfEmpty(0, -1)); break;

        case Key::Char:
            if (event.character >= '1' && event.character <= '8') {
                tryMove(event.character - '0');
            }
            break;

        case Key::Escape:
            m_gameRouter->menu();
            break;

        default:
            break;
    }
}
