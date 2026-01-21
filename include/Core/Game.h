//
// Created by Oier Álvarez on 18/01/2026.
//

#ifndef SPACESTATIONDEFENSE_GAME_H
#define SPACESTATIONDEFENSE_GAME_H

#include <cstdint>
#include <memory>
#include <string>

namespace sf
{
    class RenderWindow;
}

class World;

class Game
{
public:

    struct GameCreateInfo
    {
        std::string gameTitle;
        uint32_t screenWidth;
        uint32_t screenHeight;
        uint32_t frameRateLimit;
    };

    ~Game();

    bool init(GameCreateInfo& createInfo);

    [[nodiscard]] bool isRunning() const;

    void update(uint32_t deltaMilliseconds);
    void render();

    World* getWorld() const;
    sf::RenderWindow& getWindow();
    const sf::RenderWindow& getWindow() const;

private:

    sf::RenderWindow* m_window{ nullptr };
    std::unique_ptr<World> m_world{ nullptr };
};

#endif //SPACESTATIONDEFENSE_GAME_H