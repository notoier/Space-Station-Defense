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
    void render(sf::RenderWindow& window) const override;

    void resume();
    void quit();
    void settings();
    void setResumeFunc(const std::function<void()>& func);
    void setSettingsFunc(const std::function<void()>& func);
    void setExitFunc(const std::function<void()>& func);
private:
    std::function<void()> m_onResume = nullptr;
    std::function<void()> m_onSettings = nullptr;
    std::function<void()> m_onExit = nullptr;

    sf::Vector2f m_position {0, 0};
    sf::RectangleShape m_window_shape;
};


#endif //SPACESTATIONDEFENSE_PAUSEMENU_H