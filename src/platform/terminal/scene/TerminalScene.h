#pragma once

#include <cengine/core/IScene.hpp>

// Base das cenas de terminal do 8Puzzle.
//
// A cengine::core::IScene declara onEnterExecuted()/isOnEnterExecuted() como
// puros; esta base fornece o rastreamento de "onEnter já executado" (o mesmo
// comportamento que o antigo IScene vendorizado trazia por padrão), para que
// cada cena precise implementar apenas onEnter/draw/input/onExit.
class TerminalScene : public cengine::core::IScene {
    bool m_isEnterExecuted = false;

public:
    void onEnterExecuted() override { m_isEnterExecuted = true; }
    [[nodiscard]] bool isOnEnterExecuted() const override { return m_isEnterExecuted; }
};
