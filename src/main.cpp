#include "Core/Game.h"
#include "Core/World.h"

#include "Gameplay/Entity.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/Station.h"
#include "Utils/Constants.h"

#include <SFML/System/Clock.hpp>

#include <cstdlib>   // rand
#include <cstdint>

int main()
{
    Game game;

    Game::GameCreateInfo ci;
    ci.gameTitle = "SpaceStationDefense";
    ci.screenWidth = SCREEN_WIDTH;
    ci.screenHeight = SCREEN_HEIGHT;
    ci.frameRateLimit = 60;

    if (!game.init(ci))
        return 1;

    World* world = game.getWorld();
    if (!world)
        return 1;

    // ========= STATION =========
    Station::StationDescriptor stationDesc;
    stationDesc.position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
    stationDesc.health = 5;
    stationDesc.visualType = Entity::VisualType::StationCircle;
    stationDesc.radius = 50.f;

    auto station = std::make_unique<Station>();
    station->init(stationDesc);
    world->setStation(std::move(station));

    // ========= ENEMIES =========
    Enemy::EnemyDescriptor enemyDesc;
    enemyDesc.health = 5;
    enemyDesc.visualType = Entity::VisualType::BasicEnemySquares;
    enemyDesc.velocity = sf::Vector2f(60.f, 0.f);

    world->spawnEnemy(enemyDesc, stationDesc.position, stationDesc.radius);

    sf::Clock clock;

    while (game.isRunning())
    {
        const uint32_t deltaMs = static_cast<uint32_t>(clock.restart().asMilliseconds());
        game.update(deltaMs);
        game.render();
    }

    return 0;
}
