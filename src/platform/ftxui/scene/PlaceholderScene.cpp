#include "PlaceholderScene.h"

#include <utility>

#include "../Keyboard.h"

using namespace ftxui;

PlaceholderScene::PlaceholderScene(std::string title, std::function<void()> goBack)
    : m_title(std::move(title)), m_goBack(std::move(goBack)) {}

void PlaceholderScene::draw() {
    auto content = vbox({
        text(" "),
        text(m_title) | bold | color(Color::Yellow) | hcenter,
        text(" "),
        text("Esta tela ainda não foi portada para o FTXUI (PoC).") | hcenter,
        text(" "),
    });

    present(vbox({
        filler(),
        window(text(" em construção "), content) | size(WIDTH, EQUAL, 58) | hcenter,
        filler(),
        hints("ESC voltar ao menu"),
    }));
}

void PlaceholderScene::input() {
    if (readKey() == Key::Escape) {
        m_goBack();
    }
}
