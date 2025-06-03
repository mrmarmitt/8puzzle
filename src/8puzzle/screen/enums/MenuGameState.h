#pragma once
#include <string>
#include <unordered_map>

enum class MenuGameState { None, NewGame, LoadGame, Records, Settings, Exit };

extern const std::unordered_map<MenuGameState, std::string> menuLabels;