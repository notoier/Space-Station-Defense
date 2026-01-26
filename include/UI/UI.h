//
// Created by Oier Álvarez on 26/01/2026.
//

#ifndef SPACESTATIONDEFENSE_UI_H
#define SPACESTATIONDEFENSE_UI_H
#include "Render/CompositeShape.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"


class UI
{

public:
    UI() = default;
    ~UI() = default;
    void init();
    void render(sf::RenderWindow& window);

    void healthDown(float healthPercentage);
    void healthUp(float healthPercentage);

    void barrierDown(float barrierPercentage);
    void barrierUp(float barrierPercentage);

    void updateMoney(float money);
private:
    CompositeShape m_healthBar;
    CompositeShape m_barrierBar;
    sf::Text m_moneyText;
};


#endif //SPACESTATIONDEFENSE_UI_H