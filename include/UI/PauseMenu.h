//
// Created by Oier Álvarez on 21/01/2026.
//

#ifndef SPACESTATIONDEFENSE_PAUSEMENU_H
#define SPACESTATIONDEFENSE_PAUSEMENU_H
#include "Menu.h"
#include "SFML/Graphics/RectangleShape.hpp"


class PauseMenu : public Menu
{
public:
    bool init() override;
    bool resume();
    bool quit();
    bool settings();

private:
    sf::Vector2f m_position {0, 0};
    sf::RectangleShape m_window_shape;
};


#endif //SPACESTATIONDEFENSE_PAUSEMENU_H