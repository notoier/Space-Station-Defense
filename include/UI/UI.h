//
// Created by Oier Álvarez on 26/01/2026.
//

#ifndef SPACESTATIONDEFENSE_UI_H
#define SPACESTATIONDEFENSE_UI_H
#include "Menu.h"
#include "Render/CompositeShape.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"


class UI : public Menu
{

public:
    UI() = default;
    ~UI() override = default;
    bool init() override;
    void initHealthBar(sf::Vector2f pos);
    void initBarrierBar(sf::Vector2f pos);
    void initText();
    void render(sf::RenderWindow& window) const override;

    void healthDown(float healthPercentage);
    void healthUp(float healthPercentage);

    void barrierDown(float barrierPercentage);
    void barrierUp(float barrierPercentage);

    void updateCurrency(int money);
private:
    CompositeShape m_healthBar;
    CompositeShape m_barrierBar;
    sf::Text m_moneyText;
};


#endif //SPACESTATIONDEFENSE_UI_H