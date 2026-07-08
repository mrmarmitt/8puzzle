#include "FtxuiExitScene.h"

using namespace ftxui;

void FtxuiExitScene::draw() {
    present(vbox({
        filler(),
        text("Até logo!") | bold | color(Color::Cyan) | hcenter,
        text(" "),
        text("Obrigado por jogar 8 · PUZZLE") | hcenter,
        filler(),
    }));
}
