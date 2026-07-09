#pragma once

#include <cengine/core/IScene.hpp>

#include "../ForgeUi.h"

// Base das cenas The-Forge: renderizacao em modo imediato — cada draw()
// emite texto direto no command buffer do quadro (publicado em forgeui).
// Defaults vazios para os metodos de ciclo de vida que a maioria nao usa,
// como na FtxuiScene.
class ForgeScene : public cengine::core::IScene {
public:
    void onEnter() override {}
    void update(cengine::core::Seconds) override {}
    void onExit() override {}
};
