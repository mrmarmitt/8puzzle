#include "FtxuiGameScene.h"

#include <chrono>
#include <string>
#include <utility>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "../BoardView.h"
#include "../Format.h"
#include "../Keyboard.h"

using namespace ftxui;

FtxuiGameScene::FtxuiGameScene(std::shared_ptr<GameRouter> gameRouter,
                               std::shared_ptr<GamePlayService> gamePlayService)
    : m_gameRouter(std::move(gameRouter)),
      m_gamePlayService(std::move(gamePlayService)) {}

void FtxuiGameScene::draw() {
    const auto& gamePlay = m_gamePlayService->getGamePlay();

    // O tempo exibido é o mesmo que vale para o recorde: relógio de parede
    // desde o início da partida (dt é tempo de simulação; não serve aqui).
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - gamePlay.getStartedAt()).count();

    auto stats = vbox({
        text("Movimentos") | dim,
        text(std::to_string(gamePlay.getNumberOfMoves())) | bold | color(Color::Yellow),
        text(" "),
        text("Tempo") | dim,
        text(formatMillis(elapsed)) | bold | color(Color::Yellow),
    });

    auto content = hbox({
        boardElement(m_board),
        text("   "),
        stats | vcenter,
    });

    present(vbox({
        filler(),
        window(text(" 8 · PUZZLE — jogo "), content) | hcenter,
        filler(),
        hints("↑/↓/←/→ deslizar peça   1-8 mover pelo número   ESC menu"),
    }));
}

int FtxuiGameScene::neighborOfEmpty(const int dLine, const int dColumn) const {
    const auto [emptyLine, emptyColumn] = m_board.getEmptyPosition();
    const int line = emptyLine + dLine;
    const int column = emptyColumn + dColumn;

    if (line < 0 || line > 2 || column < 0 || column > 2) {
        return -1;
    }
    return m_board.getBoard()[line * 3 + column].getId();
}

void FtxuiGameScene::tryMove(const int tileId) {
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

void FtxuiGameScene::input() {
    const KeyEvent event = readKey();

    switch (event.key) {
        // A seta indica a direção em que a PEÇA desliza: ↑ move a peça que
        // está ABAIXO do vazio, e assim por diante.
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
