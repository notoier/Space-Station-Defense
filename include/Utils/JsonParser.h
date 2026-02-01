//
// Created by Oier Álvarez on 01/02/2026.
//

#ifndef SPACESTATIONDEFENSE_JSONPARSER_H
#define SPACESTATIONDEFENSE_JSONPARSER_H
#include <string>
#include "Gameplay/Upgrades.h"
#include "nlohmann/json.hpp"
#include "SFML/System/Vector2.hpp"
using json = nlohmann::json;

enum class UpgradeId;
// ReSharper disable once CppDFAConstantParameter

/* Get int or default value from json */
int getIntOr(const json& j, const char* key, const int def);

/* Get float or default value from json */
float getFloatOr(const json& j, const char* key, const float def);

/* Get string or default value from json */
std::string getStringOr(const json& j, const char* key, const std::string& def);

/* Get vector or default value from json */
sf::Vector2f getVectorOr(const json& j, const char* key, sf::Vector2f& def);

bool tryParseUpgradeId(const std::string& s, UpgradeId& out);

#endif //SPACESTATIONDEFENSE_JSONPARSER_H