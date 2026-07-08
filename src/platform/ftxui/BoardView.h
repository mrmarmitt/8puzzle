#pragma once

#include <string>

#include <ftxui/dom/elements.hpp>

class Board;

// Visual compartilhado do tabuleiro (splash decorativa + jogo de verdade).

// Uma célula do tabuleiro; label vazio = espaço vazio do puzzle.
ftxui::Element tileCell(const std::string& label);

// O tabuleiro 3x3 real, renderizado a partir do estado do domínio.
ftxui::Element boardElement(const Board& board);
