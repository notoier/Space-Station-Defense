//
// Created by Oier Álvarez on 21/01/2026.
//

#include "UI/PauseMenu.h"

#include <iostream>
#include <ostream>
#include <utility>

#include "UI/Button.h"
#include "Utils/Constants.h"
#include "Utils/MathUtils.h"

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

    const sf::Vector2f buttonSize{
        size.x * BUTTON_SIZE_PERCENTAGE.x,
        size.y * BUTTON_SIZE_PERCENTAGE.y
    };

    sf::RectangleShape buttonShape(buttonSize);
    buttonShape.setOutlineThickness(2.f);
    buttonShape.setOutlineColor(OUTLINE_COLOR);

    constexpr int buttonCount = 3;
    const float spacing = size.y * 0.04f;
    const float totalHeight = buttonCount * buttonSize.y + (buttonCount - 1) * spacing;

    const float startY = m_position.y + (size.y - totalHeight) * 0.5f;

    const float x = m_position.x + (size.x - buttonSize.x) * 0.5f;

    if (sf::Font font; font.loadFromFile("data/fonts/rush_driver_italic.otf"))
    {
        m_font = font;
        m_text.scale(2,2);
        m_text.setFont(m_font);
        m_text.setString("Paused");
        centerText(m_text, m_window_shape);

        sf::Vector2f fontPosition = {m_text.getPosition().x , m_text.getPosition().y - m_window_shape.getSize().y * 0.35f};
        m_text.setPosition(fontPosition);
    }
    else
    {
        std::cerr << "Font loading failed" << std::endl;
    }

    // Create buttons
    createButton({x, startY + 0 * (buttonSize.y + spacing)}, buttonShape, labels[0], [this]() { return resume(); });
    createButton({x, startY + 1 * (buttonSize.y + spacing)}, buttonShape, labels[1], [this]() { return settings(); });
    createButton({x, startY + 2 * (buttonSize.y + spacing)}, buttonShape, labels[2], [this]() { return quit(); });

    return true;
}

void PauseMenu::resume()
{
    std::cout << "PauseMenu::resume" << std::endl;
    if (m_onResume) m_onResume();
}

void PauseMenu::quit()
{
    std::cout << "PauseMenu::quit()" << std::endl;
    if (m_onExit) m_onExit();
}

void PauseMenu::settings()
{
    std::cout << "PauseMenu::settings()" << std::endl;
    if (m_onSettings) m_onSettings();
}

void PauseMenu::setResumeFunc(const std::function<void()>& func)
{
    m_onResume = func;
}

void PauseMenu::setSettingsFunc(const std::function<void()>& func)
{
    m_onSettings = func;
}

void PauseMenu::setExitFunc(const std::function<void()>& func)
{
    m_onExit = func;
}

void PauseMenu::render(sf::RenderWindow& window) const
{
    if (!m_enabled) return;

    window.draw(m_window_shape);
    window.draw(m_text);

    for (auto& button : m_buttons)
    {
        button->render(window);
    }
}



