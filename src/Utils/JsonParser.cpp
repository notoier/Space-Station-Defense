//
// Created by Oier Álvarez on 01/02/2026.
//

#include "Utils/JsonParser.h"

/* Get int or default value from json */
int getIntOr(const json& j, const char* key, const int def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_number_integer()) return def;
    return j[key].get<int>();
}

/* Get float or default value from json */
float getFloatOr(const json& j, const char* key, const float def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_number()) return def;
    return j[key].get<float>();
}

/* Get string or default value from json */
std::string getStringOr(const json& j, const char* key, const std::string& def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_string()) return def;
    return j[key].get<std::string>();
}

/* Get vector or default value from json */
sf::Vector2f getVectorOr(const json& j, const char* key, sf::Vector2f& def)
{
    if (!j.contains(key) || !j[key].is_object())
        return def;

    const json& v = j[key];
    def.x = getFloatOr(v, "x", def.x);
    def.y = getFloatOr(v, "y", def.y);
    return def;
}

/* Not really json related but ok */
bool tryParseUpgradeId(const std::string& s, UpgradeId& out)
{
    if (s == "Laser_Damage")         { out = UpgradeId::Laser_Damage; return true; }
    if (s == "Laser_Range")          { out = UpgradeId::Laser_Range; return true; }

    if (s == "Barrier_MaxHealth")    { out = UpgradeId::Barrier_MaxHealth; return true; }
    if (s == "Barrier_RegenAmount")  { out = UpgradeId::Barrier_RegenAmount; return true; }

    if (s == "Cannon_Damage")        { out = UpgradeId::Cannon_Damage; return true; }
    if (s == "Cannon_FireRate")      { out = UpgradeId::Cannon_FireRate; return true; }

    return false;
}