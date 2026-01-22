//
// Created by Oier Álvarez on 21/01/2026.
//

#ifndef SPACESTATIONDEFENSE_PAUSEMENU_H
#define SPACESTATIONDEFENSE_PAUSEMENU_H
#include "UI/Menu.h"
#include "SFML/Graphics/RectangleShape.hpp"


class PauseMenu : public Menu
{
public:
    PauseMenu();
    bool init() override;
    bool resume();
    bool quit();
    bool settings();

    void render(sf::RenderWindow& window) const override;

private:
    sf::Vector2f m_position {0, 0};
    sf::RectangleShape m_window_shape;
};


#endif //SPACESTATIONDEFENSE_PAUSEMENU_H