//
// Created by Oier Álvarez on 19/01/2026.
//

#ifndef SPACESTATIONDEFENSE_STATION_H
#define SPACESTATIONDEFENSE_STATION_H
#include "Entity.h"
#include "Weapon.h"

class Weapon;

class Station : public Entity
{
public:
    struct StationDescriptor : EntityDescriptor
    {
        float radius = 0.f;
    };

    bool init(const StationDescriptor& descriptor);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    std::vector<std::unique_ptr<Weapon>>& getWeapons();
    const std::vector<std::unique_ptr<Weapon>>& getWeapons() const;
    void addWeapon(std::unique_ptr<Weapon> weapon);
    void shootLaser();

    void setAimWorld(const sf::Vector2f& aim);
    [[nodiscard]] const sf::Vector2f& getAimWorld() const;
    void onLeftClick();

protected:
    std::vector<std::unique_ptr<Weapon>> m_weapons;
    sf::Vector2f m_aimWorld {0.f, 0.f};
};


#endif //SPACESTATIONDEFENSE_STATION_H