#include "MenuGameState.h"

const std::unordered_map<MenuGameState, std::string> menuLabels = {
    {MenuGameState::NewGame, "Novo Jogo"},
    {MenuGameState::LoadGame, "Carregar Jogo"},
    {MenuGameState::Records, "Recordes"},
    {MenuGameState::Settings, "Configuracoes"},
    {MenuGameState::Exit, "Sair"}
};