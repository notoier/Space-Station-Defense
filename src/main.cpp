#include "Core/Game.h"
#include "Core/World.h"

#include "Gameplay/Entity.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/Station.h"
#include "Utils/Constants.h"

#include <SFML/System/Clock.hpp>

#include <cstdlib>   // rand
#include <cstdint>

#include "Gameplay/Cannon.h"
#include "Gameplay/Laser.h"

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
    stationDesc.health = 20.f;
    stationDesc.visualType = Entity::VisualType::StationCircle;
    stationDesc.radius = 50.f;

    auto station = std::make_unique<Station>();
    station->init(stationDesc);

    // Create laser weapon and add it to the station
    Laser::LaserBaseStats laserStats;
    laserStats.range = 600.f;
    laserStats.damage = 0.5f;
    laserStats.width = 25.f;

    station->addWeapon(std::make_unique<Laser>(laserStats)); // Laser active by default

    Cannon::CannonDesc cannonStats;
    cannonStats.damage = 0.5f;
    cannonStats.fireCooldownSec = 1.f;
    cannonStats.orbitRadius = 75.f;
    cannonStats.targetMaxDistance = 10000.f;
    cannonStats.orbitSpeedDegPerSec = 90.f;
    cannonStats.projectileSpeed = 2.f;

    station->addWeapon(std::make_unique<Cannon>(cannonStats));

    world->setStation(std::move(station));

    // ========= ENEMIES =========
    Enemy::EnemyDescriptor enemyDesc;
    enemyDesc.health = 1;
    enemyDesc.visualType = Entity::VisualType::BasicEnemySquares;
    enemyDesc.velocity = sf::Vector2f(60.f, 0.f);

    World::Wave wave1;
    wave1.amountOfEnemies = 10;
    wave1.descriptor = enemyDesc;

    world->addWave(wave1);
    world->spawnEnemy(stationDesc.position, stationDesc.radius);

    sf::Clock clock;

    while (game.isRunning())
    {
        const uint32_t deltaMs = static_cast<uint32_t>(clock.restart().asMilliseconds());
        game.update(deltaMs);
        game.render();
    }

    return 0;
}
