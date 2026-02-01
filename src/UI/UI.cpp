//
// Created by Oier Álvarez on 26/01/2026.
//

#include "UI/UI.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Utils/Constants.h"
#include "Utils/MathUtils.h"

bool UI::init()
{
    /* BARRIER BAR */
    const sf::Vector2f barrierBarPos = {1400, 950 };
    initBarrierBar(barrierBarPos);

    /* HEALTH BAR */
    const sf::Vector2f healthBarPos = barrierBarPos + sf::Vector2f{0,50};
    initHealthBar(healthBarPos);

    initText();

    return true;
}

void UI::initHealthBar(const sf::Vector2f pos)
{
    auto healthBarOutline = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE);;
    healthBarOutline->setFillColor(sf::Color::Transparent);
    healthBarOutline->setOutlineColor(OUTLINE_COLOR);
    healthBarOutline->setOutlineThickness(1.0f);

    auto healthBarFill = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE  - sf::Vector2f{5, 5});
    healthBarFill->setFillColor(HEALTH_BAR_COLOR);

    m_healthBar.addPart(std::move(healthBarOutline), 100, {-2.5, -2.5});
    m_healthBar.addPart(std::move(healthBarFill), 100, {0,0});
    m_healthBar.setPosition(pos);
}

void UI::initBarrierBar(const sf::Vector2f pos)
{
    auto barrierBarOutline = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE);;
    barrierBarOutline->setFillColor(sf::Color::Transparent);
    barrierBarOutline->setOutlineColor(OUTLINE_COLOR);
    barrierBarOutline->setOutlineThickness(1.0f);

    auto barrierBarFill = std::make_unique<sf::RectangleShape>(UI_BAR_SIZE - sf::Vector2f{5, 5});
    barrierBarFill->setFillColor(BARRIER_BAR_COLOR);

    m_barrierBar.addPart(std::move(barrierBarOutline), 100, {-2.5, -2.5});
    m_barrierBar.addPart(std::move(barrierBarFill), 100, {0,0});
    m_barrierBar.setPosition(pos);
}

void UI::initText()
{
    initFont("mc.ttf");
    m_text.setFont(m_font);
    m_text.setString("$: 0");
    m_text.scale(2,2);

    const sf::Vector2f fontPosition = {(SCREEN_WIDTH - m_text.getGlobalBounds().width) * 0.5f, m_text.getPosition().y + m_text.getGlobalBounds().height};
    m_text.setPosition(fontPosition);
}

void UI::render(sf::RenderWindow& window) const
{
    window.draw (m_healthBar);
    window.draw (m_barrierBar);
    if (!m_text.getString().isEmpty()) window.draw (m_text);
}

void UI::healthDown(const float healthPercentage)
{
    sf::Vector2f newBarSize = (UI_BAR_SIZE - sf::Vector2f{5, 5});
    newBarSize.x *= healthPercentage;
    if (CompositeShape::Part* part = m_healthBar.getPart(1); part != nullptr)
    {
        part->shape = std::make_unique<sf::RectangleShape>(newBarSize);
        part->shape->setFillColor(HEALTH_BAR_COLOR);
    }
}

void UI::healthUp(const float healthPercentage)
{
    sf::Vector2f newBarSize = (UI_BAR_SIZE - sf::Vector2f{5, 5});
    newBarSize.x *= healthPercentage;
    if (CompositeShape::Part* part = m_healthBar.getPart(1); part != nullptr)
    {
        part->shape = std::make_unique<sf::RectangleShape>(newBarSize);
        part->shape->setFillColor(HEALTH_BAR_COLOR);
    }
}

void UI::barrierDown(const float barrierPercentage)
{
    sf::Vector2f newBarSize = (UI_BAR_SIZE - sf::Vector2f{5, 5});
    newBarSize.x *= barrierPercentage;
    if (CompositeShape::Part* part = m_barrierBar.getPart(1); part != nullptr)
    {
        part->shape = std::make_unique<sf::RectangleShape>(newBarSize);
        part->shape->setFillColor(BARRIER_BAR_COLOR);
    }
}

void UI::barrierUp(const float barrierPercentage)
{
    sf::Vector2f newBarSize = (UI_BAR_SIZE - sf::Vector2f{5, 5});
    newBarSize.x *= barrierPercentage;
    if (CompositeShape::Part* part = m_barrierBar.getPart(1); part != nullptr)
    {
        part->shape = std::make_unique<sf::RectangleShape>(newBarSize);
        part->shape->setFillColor(BARRIER_BAR_COLOR);
    }
}

void UI::reset()
{
    barrierUp(100);
    healthUp(100);
}

void UI::updateCurrency(const int money)
{
    m_text.setString("$: " + std::to_string(money));
}

