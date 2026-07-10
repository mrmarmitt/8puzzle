#pragma once

#include <chrono>

#include <cengine/core/IWindowManager.hpp>

// Plataforma FTXUI (modo imediato): prepara o console para ANSI/UTF-8, limita
// o loop a ~60 quadros/s (o pacing é responsabilidade da plataforma — ver
// task 14 da cengine) e reposiciona o cursor no início de cada quadro; as
// cenas reescrevem a tela inteira via FtxuiScene::present().
class FtxuiWindowManager final : public cengine::core::IWindowManager {
    using Clock = std::chrono::steady_clock;
    static constexpr std::chrono::milliseconds kFramePeriod{16}; // ~60 FPS

    Clock::time_point m_nextFrame;

public:
    void init() override;
    void update() override;
    // Vazio (cengine 0.5.0): a apresentação já acontece no draw das cenas
    // (FtxuiScene::present() imprime a tela). Migrar o Print() para cá é
    // melhoria opcional registrada na task 16 da cengine.
    void present() override {}
    void cleanup() override;
};
