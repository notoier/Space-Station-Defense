//
// Created by Oier Álvarez on 01/02/2026.
//

#ifndef SPACESTATIONDEFENSE_GAMEOVERMENU_H
#define SPACESTATIONDEFENSE_GAMEOVERMENU_H
#include "Menu.h"


class GameOverMenu : public Menu
{
public:
    bool init() override;
    void render(sf::RenderWindow& window) const override;
    void setNewGameFunction(const std::function<void()>& func);
    void setQuitGameFunction(const std::function<void()>& func);

private:
    std::function<void()> m_newGame;
    std::function<void()> m_quitGame;
};


#endif //SPACESTATIONDEFENSE_GAMEOVERMENU_H