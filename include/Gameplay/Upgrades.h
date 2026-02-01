//
// Created by Oier Álvarez on 30/01/2026.
//

#ifndef SPACESTATIONDEFENSE_UPGRADEID_H
#define SPACESTATIONDEFENSE_UPGRADEID_H

enum class UpgradeId
{
    Laser_Damage,
    Laser_Range,

    Barrier_MaxHealth,
    Barrier_RegenAmount,

    Cannon_Damage,
    Cannon_FireRate
};

struct UpgradeDefinition
{
    std::string uiName;
    int baseCost = 0;
    float costScale = 1.f;
    int maxLevel = 1;

    float delta = 0.f;  /* Effect upgrade */
};

struct UpgradeState
{
    int level = 0;
};

#endif //SPACESTATIONDEFENSE_UPGRADEID_H