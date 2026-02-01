#include "UI/UpgradeMenu.h"
#include "UI/Button.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "Gameplay/Weapon.h"
#include "Utils/Constants.h"

bool UpgradeMenu::init()
{
    if (!Menu::init())
        return false;

    m_enabled = true;

    /*  MENU TITLE */
    m_title.setFont(m_font);
    m_title.setString("UPGRADES");
    m_title.setCharacterSize(48);
    m_title.setFillColor(OUTLINE_COLOR);

    const sf::FloatRect titleBounds = m_title.getLocalBounds();
    m_title.setOrigin(titleBounds.width * 0.5f, titleBounds.height * 0.5f);
    m_title.setPosition(960.f, 120.f); // screen center top

    /* BORDER */
    m_border.setSize({ 1200.f, 700.f });
    m_border.setPosition({ 360.f, 80.f });
    m_border.setFillColor(sf::Color::Transparent);
    m_border.setOutlineColor(OUTLINE_COLOR);
    m_border.setOutlineThickness(3.f);

    /*  COLUMNS */
    constexpr float screenCenterX = 960.f;
    constexpr float columnOffset  = 350.f;
    constexpr float startY        = 260.f;

    createColumn("LASER",   screenCenterX - columnOffset, startY, Weapon::Laser);
    createColumn("BARRIER", screenCenterX,                startY, Weapon::Barrier);
    createColumn("CANNON",  screenCenterX + columnOffset, startY, Weapon::Cannon);

    return true;
}

void UpgradeMenu::createColumn(const std::string& title, const float centerX, const float startY, const Weapon::WeaponType type)
{
    sf::Text text;
    text.setFont(m_font);
    text.setString(title);
    text.setCharacterSize(36);
    text.setFillColor(OUTLINE_COLOR);

    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    text.setPosition(centerX, startY);

    constexpr float BUTTON_WIDTH  = 300.f;
    constexpr float BUTTON_HEIGHT = 90.f;

    m_columnTitles.push_back(text);

    sf::RectangleShape buttonShape;
    buttonShape.setSize({ BUTTON_WIDTH, BUTTON_HEIGHT });
    buttonShape.setFillColor(sf::Color(60, 60, 60));

    const float buttonStartY = startY + 80.f;
    constexpr float buttonSpacing = BUTTON_HEIGHT + 50.f;
    sf::Vector2f buttonPos = { centerX - BUTTON_WIDTH * 0.5f, buttonStartY };

    UpgradeId upgrade1 = ((type == Weapon::Laser) ? UpgradeId::Laser_Damage : ((type == Weapon::Cannon) ? UpgradeId::Cannon_Damage : UpgradeId::Barrier_MaxHealth));
    UpgradeId upgrade2 = ((type == Weapon::Laser) ? UpgradeId::Laser_Range : ((type == Weapon::Cannon) ? UpgradeId::Cannon_FireRate : UpgradeId::Barrier_RegenAmount));

    m_buttonTexts[upgrade1] = "Loading...";
    Button* b1 = createButton(buttonPos, buttonShape,m_buttonTexts[upgrade1].c_str(),[this, upgrade1]()
        {
            if (m_onUpgradeSelected) m_onUpgradeSelected(upgrade1);
        }
    );
    m_upgradeButtons[upgrade1] = b1;

    m_buttonTexts[upgrade2] = "Loading...";
    Button* b2 = createButton({buttonPos.x, buttonPos.y + buttonSpacing}, buttonShape,
        m_buttonTexts[upgrade2].c_str(),
    [this, upgrade2]()
                {
                    if (m_onUpgradeSelected) m_onUpgradeSelected(upgrade2);
                }
    );
    m_upgradeButtons[upgrade2] = b2;

}

void UpgradeMenu::render(sf::RenderWindow& window) const
{
    if (!m_enabled)
        return;

    window.draw(m_border);
    window.draw(m_title);

    for (const auto& text : m_columnTitles)
        window.draw(text);

    for (const auto& button : m_buttons)
        button->render(window);
}

void UpgradeMenu::setOnUpgradeSelected(const std::function<void(UpgradeId)>& fn)
{
    m_onUpgradeSelected = fn;
}

void UpgradeMenu::setGetUpgradeTextFn(const std::function<std::string(UpgradeId)>& fn)
{
    m_getUpgradeText = fn;
    refreshTexts();
}

void UpgradeMenu::refreshTexts()
{
    if (!m_getUpgradeText)
        return;

    for (auto& [id, button] : m_upgradeButtons)
    {
        if (!button) continue;

        m_buttonTexts[id] = m_getUpgradeText(id);
        button->setText(m_buttonTexts[id].c_str());
    }
}
