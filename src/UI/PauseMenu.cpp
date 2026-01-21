//
// Created by Oier Álvarez on 21/01/2026.
//

#include "UI/PauseMenu.h"

#include <iostream>
#include <ostream>

#include "Utils/Constants.h"

bool PauseMenu::init()
{
    const char* text[3] = {"Resume", "Settings", "Quit"};
    m_position = sf::Vector2f(SCREEN_WIDTH * PAUSE_MENU_SIZE_PERCENTAGE.x,
                              SCREEN_HEIGHT * (1 - PAUSE_MENU_SIZE_PERCENTAGE.y) * 0.5);
    const auto size = sf::Vector2f(SCREEN_WIDTH * PAUSE_MENU_SIZE_PERCENTAGE.x,
                                     SCREEN_HEIGHT * PAUSE_MENU_SIZE_PERCENTAGE.y);

    auto shape = sf::RectangleShape(size);
    shape.setPosition(m_position);

    sf::Vector2f buttonPosition = {m_position.x * BUTTON_SIZE_PERCENTAGE.x, m_position.y * BUTTON_SIZE_PERCENTAGE.y};
    sf::Vector2f buttonSize = {size.x * BUTTON_SIZE_PERCENTAGE.x, size.y * BUTTON_SIZE_PERCENTAGE.y};

    auto buttonShape = sf::RectangleShape(buttonSize);

    createButton(buttonPosition, buttonShape, text[0], [this]() { return resume(); });
}

bool PauseMenu::resume()
{
    std::cout << "PauseMenu::resume" << std::endl;
}

bool PauseMenu::quit()
{
    std::cout << "PauseMenu::quit()" << std::endl;
}

bool PauseMenu::settings()
{
    std::cout << "PauseMenu::settings()" << std::endl;
}


