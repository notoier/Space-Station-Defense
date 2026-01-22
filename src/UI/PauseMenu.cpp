//
// Created by Oier Álvarez on 21/01/2026.
//

#include "UI/PauseMenu.h"

#include <iostream>
#include <ostream>

#include "UI/Button.h"
#include "Utils/Constants.h"

PauseMenu::PauseMenu()
{
    PauseMenu::init();
}

bool PauseMenu::init()
{
    std::cout << "PauseMenu::init" << std::endl;
    const char* labels[3] = {"Resume", "Settings", "Quit"};

    m_position = sf::Vector2f(SCREEN_WIDTH * PAUSE_MENU_SIZE_PERCENTAGE.x,
                              static_cast<float>(SCREEN_HEIGHT * (1 - PAUSE_MENU_SIZE_PERCENTAGE.y) * 0.5));

    const auto size = sf::Vector2f(SCREEN_WIDTH * PAUSE_MENU_SIZE_PERCENTAGE.x,
                                     SCREEN_HEIGHT * PAUSE_MENU_SIZE_PERCENTAGE.y);

    m_window_shape.setSize(size);
    m_window_shape.setPosition(m_position);
    m_window_shape.setFillColor(sf::Color(0, 0, 0, 160));
    m_window_shape.setOutlineThickness(2.f);
    m_window_shape.setOutlineColor(OUTLINE_COLOR);


    // Button size (relative to panel)
    const sf::Vector2f buttonSize{
        size.x * BUTTON_SIZE_PERCENTAGE.x,
        size.y * 0.12f // 12% of panel height per button (adjust if needed)
    };

    sf::RectangleShape buttonShape(buttonSize);
    buttonShape.setOutlineThickness(2.f);
    buttonShape.setOutlineColor(OUTLINE_COLOR);

    // Column layout inside panel
    const int buttonCount = 3;
    const float spacing = size.y * 0.04f; // vertical spacing
    const float totalHeight = buttonCount * buttonSize.y + (buttonCount - 1) * spacing;

    // Starting Y so the whole column is vertically centered inside the panel
    const float startY = m_position.y + (size.y - totalHeight) * 0.5f;

    // X so buttons are centered horizontally inside the panel
    const float x = m_position.x + (size.x - buttonSize.x) * 0.5f;

    // Create buttons
    createButton({x, startY + 0 * (buttonSize.y + spacing)}, buttonShape, labels[0], [this]() { return resume(); });
    createButton({x, startY + 1 * (buttonSize.y + spacing)}, buttonShape, labels[1], [this]() { return settings(); });
    createButton({x, startY + 2 * (buttonSize.y + spacing)}, buttonShape, labels[2], [this]() { return quit(); });


    return true;
}

bool PauseMenu::resume()
{
    std::cout << "PauseMenu::resume" << std::endl;
    return true;
}

bool PauseMenu::quit()
{
    std::cout << "PauseMenu::quit()" << std::endl;
    return true;
}

bool PauseMenu::settings()
{
    std::cout << "PauseMenu::settings()" << std::endl;
    return true;
}

void PauseMenu::render(sf::RenderWindow& window) const
{
    if (!m_enabled) return;

    window.draw(m_window_shape);

    for (auto& button : m_buttons)
    {
        button->render(window);
    }
}


