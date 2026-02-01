//
// Created by Oier Álvarez on 01/02/2026.
//

#include "../../include/UI/GameOverMenu.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "UI/Button.h"
#include "Utils/Constants.h"

bool GameOverMenu::init()
{
    const bool b = Menu::init();
    if (b)
    {
        m_text.setFont(m_font);
        m_text.setCharacterSize(256);
        m_text.setString("Game Over");
        m_text.setFillColor(sf::Color::White);
        m_text.setPosition((SCREEN_WIDTH - m_text.getLocalBounds().width) * 0.5f , (SCREEN_HEIGHT - m_text.getLocalBounds().height - 500) * 0.5f);
    }

    constexpr float BUTTON_WIDTH = 900, BUTTON_HEIGHT = 250;
    constexpr float buttonSpacing = BUTTON_WIDTH + 50.f;
    constexpr float offsetY = 100.f;

    const sf::Vector2f centerPos = {SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5};
    const sf::Vector2f buttonPos = {centerPos.x - BUTTON_WIDTH - 25.f, centerPos.y + offsetY};

    sf::RectangleShape buttonShape;
    buttonShape.setSize({ BUTTON_WIDTH, BUTTON_HEIGHT });
    buttonShape.setFillColor(sf::Color(60, 60, 60));

    createButton(buttonPos, buttonShape, "New Game", m_newGame);

    const sf::Vector2f nextButtonPos = {buttonPos.x + buttonSpacing, buttonPos.y};
    createButton(nextButtonPos, buttonShape, "Quit Game", m_quitGame);

    return b;
}

void GameOverMenu::render(sf::RenderWindow& window) const
{
    if (!m_enabled)
        return;

    window.clear(BACKGROUND_COLOR);
    window.draw(m_text);

    for (const auto& button : m_buttons)
        button->render(window);
}

void GameOverMenu::setNewGameFunction(const std::function<void()>& func)
{
    m_newGame = func;
}

void GameOverMenu::setQuitGameFunction(const std::function<void()>& func)
{
    m_quitGame = func;
}
