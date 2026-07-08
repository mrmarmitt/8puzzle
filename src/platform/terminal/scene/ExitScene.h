#pragma once

#include <cengine/core/IScene.hpp>

class ExitScene final : public cengine::core::IScene {

public:
    ExitScene() = default;

    void onEnter() override {}
    void update(cengine::core::Seconds) override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
