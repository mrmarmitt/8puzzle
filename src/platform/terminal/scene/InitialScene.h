#pragma once
#include <memory>

#include <cengine/core/IScene.hpp>

class GameRouter;
class InitialScene final : public cengine::core::IScene {
    std::shared_ptr<GameRouter> m_gameRouter;
public:
    explicit InitialScene(std::shared_ptr<GameRouter> gameRouter);

    void onEnter() override {}
    void update(cengine::core::Seconds) override {}
    void draw() override;
    void input() override {}
    void onExit() override {}
};
