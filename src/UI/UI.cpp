//
// Created by Oier Álvarez on 26/01/2026.
//

#include "UI/UI.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Utils/Constants.h"

void UI::init()
{
    /* HEALTH BAR */
    const sf::Vector2f healthBarPos = {1400, 950 };

    auto healthBarOutline = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE);;
    healthBarOutline->setFillColor(sf::Color::Transparent);
    healthBarOutline->setOutlineColor(OUTLINE_COLOR);
    healthBarOutline->setOutlineThickness(1.0f);

    auto healthBarFill = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE  - sf::Vector2f{5, 5});
    healthBarFill->setFillColor(OUTLINE_COLOR);

    m_healthBar.addPart(std::move(healthBarOutline), 100, {-2.5, -2.5});
    m_healthBar.addPart(std::move(healthBarFill), 100, {0,0});
    m_healthBar.setPosition(healthBarPos);


    /* BARRIER BAR */
    const sf::Vector2f barrierBarPos = healthBarPos + sf::Vector2f{0,50};

    auto barrierBarOutline = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE);;
    barrierBarOutline->setFillColor(sf::Color::Transparent);
    barrierBarOutline->setOutlineColor(OUTLINE_COLOR);
    barrierBarOutline->setOutlineThickness(1.0f);

    auto barrierBarFill = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE - sf::Vector2f{5, 5});
    barrierBarFill->setFillColor(OUTLINE_COLOR);

    m_barrierBar.addPart(std::move(barrierBarOutline), 100, {-2.5, -2.5});
    m_barrierBar.addPart(std::move(barrierBarFill), 100, {0,0});
    m_barrierBar.setPosition(barrierBarPos);

    /* TODO: MONEY TEXT */
}

void UI::render(sf::RenderWindow& window)
{
    window.draw (m_healthBar);
    window.draw (m_barrierBar);
}

void UI::healthDown(const float healthPercentage)
{
    sf::Vector2f newBarSize = (UI_BAR_SIZE - sf::Vector2f{5, 5});
    newBarSize.x *= healthPercentage;
    if (CompositeShape::Part* part = m_healthBar.getPart(1); part != nullptr)
        part->shape = std::make_unique<sf::RectangleShape>(newBarSize);
}

void UI::healthUp(float health)
{
}

void UI::barrierDown(const float barrierPercentage)
{
    sf::Vector2f newBarSize = (UI_BAR_SIZE - sf::Vector2f{5, 5});
    newBarSize.x *= barrierPercentage;
    if (CompositeShape::Part* part = m_barrierBar.getPart(1); part != nullptr)
        part->shape = std::make_unique<sf::RectangleShape>(newBarSize);
}

void UI::barrierUp(float barrier)
{
}

void UI::updateMoney(float money)
{

}
