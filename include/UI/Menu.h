//
// Created by Oier Álvarez on 21/01/2026.
//

#ifndef SPACESTATIONDEFENSE_MENU_H
#define SPACESTATIONDEFENSE_MENU_H
#include <functional>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
//#include "UI/Button.h"

namespace sf
{
    class RectangleShape;
}

class Button;

class Menu
{
public:
    Menu();
    virtual ~Menu();
    virtual bool init();
    virtual void render(sf::RenderWindow& window) const = 0;

    void setMenuTitle(const sf::Text& text);
    void setTitlePosition(const sf::Vector2f& position);
    void createButton(sf::Vector2f position, const sf::RectangleShape& shape, const char* text, const std::function<void()>& func);

    void onLeftClick(sf::Vector2f mousePos);
    void enable(bool enable);
    bool isEnabled() const;

protected:
    bool m_enabled = false;
    sf::Font m_font;
    sf::Text m_text;
    std::vector<std::unique_ptr<Button>> m_buttons;
};


#endif //SPACESTATIONDEFENSE_MENU_H