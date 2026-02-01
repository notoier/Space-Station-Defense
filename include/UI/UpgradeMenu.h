#ifndef SPACESTATIONDEFENSE_UPGRADEMENU_H
#define SPACESTATIONDEFENSE_UPGRADEMENU_H

#include "UI/Menu.h"
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Gameplay/Upgrades.h"
#include "Gameplay/Weapon.h"

class UpgradeMenu : public Menu
{
public:
    bool init() override;
    void render(sf::RenderWindow& window) const override;
    void setOnUpgradeSelected(const std::function<void(UpgradeId)>& fn);
    void refreshTexts();
    void setGetUpgradeTextFn(const std::function<std::string(UpgradeId)>& fn);

private:
    std::vector<sf::Text> m_columnTitles;

    sf::Text m_title;
    sf::RectangleShape m_border;

    std::function<std::string(UpgradeId)> m_getUpgradeText;
    std::function<void(UpgradeId)> m_onUpgradeSelected = nullptr;
    std::unordered_map<UpgradeId, Button*> m_upgradeButtons;
    std::unordered_map<UpgradeId, std::string> m_buttonTexts;

    void createColumn(const std::string& title, float centerX, float startY, Weapon::WeaponType type);
};

#endif // SPACESTATIONDEFENSE_UPGRADEMENU_H
