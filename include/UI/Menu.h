//
// Created by Oier Álvarez on 21/01/2026.
//

#ifndef SPACESTATIONDEFENSE_MENU_H
#define SPACESTATIONDEFENSE_MENU_H
#include <functional>

#include "SFML/Graphics/Text.hpp"


namespace sf
{
    class RectangleShape;
}

class Button;

class Menu
{
public:
    virtual ~Menu() = default;
    virtual bool init();
    void setMenuTitle(const sf::Text& text);
    void setTitlePosition(const sf::Vector2f& position);
    void createButton(sf::Vector2f position, const sf::RectangleShape& shape, const char* text, const std::function<bool()>& func);
private:

    sf::Text m_text;
    std::vector<std::unique_ptr<Button>> m_buttons;
};


#endif //SPACESTATIONDEFENSE_MENU_H