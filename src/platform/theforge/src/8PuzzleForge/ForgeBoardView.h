#pragma once

// Desenho do tabuleiro 3x3 com o renderizador de fontes do The-Forge —
// equivalente ao BoardView da plataforma FTXUI.

#include <array>
#include <string>

class Board;

// Desenha um grid 3x3 de rotulos centrado horizontalmente em centerX, com o
// topo em topY; celula vazia ("") vira um ponto apagado.
void drawTileGrid(const std::array<std::string, 9>& labels, float centerX, float topY, float cellSize);

// Idem, a partir do estado real do Board.
void drawBoard(const Board& board, float centerX, float topY, float cellSize);

// Altura total ocupada pelo grid (para posicionar o que vem abaixo).
inline float boardHeight(const float cellSize) { return cellSize * 3.0f; }
