#ifndef SPACESTATIONDEFENSE_CONSTS_H
#define SPACESTATIONDEFENSE_CONSTS_H
#include "SFML/Graphics/Color.hpp"

// Window
constexpr auto GAME_TITLE = "GameTemplate";
constexpr int SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;

// Colors
const auto BACKGROUND_COLOR = sf::Color(33, 33, 50, 255);
const auto PAUSED_BACKGROUND_COLOR = sf::Color(5, 5, 10, 135);

const auto OUTLINE_COLOR = sf::Color(255, 255, 255);
const auto BUTTON_PRESSED_COLOR = sf::Color(200, 200, 200);

// UI Layout
const sf::Vector2f PAUSE_MENU_SIZE_PERCENTAGE = {0.33f, 0.8f};
const sf::Vector2f BUTTON_SIZE_PERCENTAGE = {0.8f, 0.12f};
const sf::Vector2f UI_BAR_SIZE = {500, 25};

// Gameplay
const sf::Vector2f STATION_POSITION = {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f};

#endif //SPACESTATIONDEFENSE_CONSTS_H