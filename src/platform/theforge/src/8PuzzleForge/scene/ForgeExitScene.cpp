#include "ForgeExitScene.h"

void ForgeExitScene::draw() {
    forgeui::drawTextCentered("Ate a proxima!", forgeui::screenHeight() * 0.45f, 32.0f,
                              forgeui::color::kTitle);
}
