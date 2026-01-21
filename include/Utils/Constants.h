#ifndef SPACESTATIONDEFENSE_CONSTS_H
#define SPACESTATIONDEFENSE_CONSTS_H
#include "SFML/Graphics/Color.hpp"

constexpr auto GAME_TITLE = "GameTemplate";

const auto BACKGROUND_COLOR = sf::Color(33, 33, 50, 255);
const auto OUTLINE_COLOR = sf::Color(255, 255, 255);
const auto BUTTON_PRESSED_COLOR = sf::Color(200, 200, 200);

constexpr int SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;

const sf::Vector2f PAUSE_MENU_SIZE_PERCENTAGE = {0.33, 0.8};
const sf::Vector2f BUTTON_SIZE_PERCENTAGE = {0.8, 0.2};

const sf::Vector2f STATION_POSITION = {SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5};

#endif //SPACESTATIONDEFENSE_CONSTS_H