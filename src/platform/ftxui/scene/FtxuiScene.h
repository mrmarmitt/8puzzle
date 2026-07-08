#pragma once

#include <cengine/core/IScene.hpp>
#include <ftxui/dom/elements.hpp>

// Base das cenas FTXUI: renderização em modo imediato — cada draw() monta um
// ftxui::Element e o apresenta por cima do quadro anterior (o cursor volta ao
// topo no FtxuiWindowManager::update()). Defaults vazios para os métodos de
// ciclo de vida que a maioria das cenas não usa.
class FtxuiScene : public cengine::core::IScene {
protected:
    // Renderiza o elemento num quadro de tamanho fixo e o imprime no console.
    static void present(ftxui::Element element);

    // Rodapé padrão com as dicas de tecla da cena.
    static ftxui::Element hints(const std::string& text);

public:
    void onEnter() override {}
    void update(cengine::core::Seconds) override {}
    void onExit() override {}
};
