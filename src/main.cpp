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

    sf::Clock clock;
    while (game.isRunning())
    {
        const uint32_t deltaMs = static_cast<uint32_t>(clock.restart().asMilliseconds());
        game.update(deltaMs);
        game.render();
    }

    return 0;
}
