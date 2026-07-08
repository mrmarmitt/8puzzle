#include "FtxuiScene.h"

#include <utility>

#include <ftxui/screen/screen.hpp>

namespace {
// Quadro fixo: reescrever sempre a mesma área evita artefatos entre cenas.
constexpr int kScreenWidth = 80;
constexpr int kScreenHeight = 26;
} // namespace

void FtxuiScene::present(ftxui::Element element) {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Fixed(kScreenWidth),
        ftxui::Dimension::Fixed(kScreenHeight));
    ftxui::Render(screen, std::move(element));
    screen.Print();
}

ftxui::Element FtxuiScene::hints(const std::string& text) {
    return ftxui::text(text) | ftxui::dim | ftxui::hcenter;
}
