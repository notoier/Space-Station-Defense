//
// Created by Oier Álvarez on 18/01/2026.
//

#ifndef SPACESTATIONDEFENSE_GAME_H
#define SPACESTATIONDEFENSE_GAME_H

#include <cstdint>
#include <memory>
#include <string>

#include "UI/GameOverMenu.h"
#include "UI/PauseMenu.h"
#include "UI/UI.h"
#include "UI/UpgradeMenu.h"

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
    void restartGame();

private:

    bool m_isPaused = false;
    bool m_upgradesOpened = false;
    bool m_isGameOver = false;

    void pauseGame();
    void resumeGame();
    void gameOver();
    void togglePause();
    void toggleUpgradeMenu();
    void closeUpgradeMenu();
    void openUpgradeMenu();
    void quitGame() const;
    void retryGame();
    void damageReceived(float healthPercentage) const;
    void barrierDamageReceived(float healthPercentage) const;
    void barrierHealthGained(float healthPercentage) const;
    void updateCurrency(int currency) const;

    PauseMenu* m_pauseWindow{nullptr};
    UI* m_ui{nullptr};
    GameOverMenu* m_gameOverWindow{nullptr};
    UpgradeMenu* m_upgradeWindow{nullptr};
    sf::RectangleShape m_pauseOverlay;
    sf::RenderWindow* m_window{ nullptr };
    std::unique_ptr<World> m_world{ nullptr };
};

#endif //SPACESTATIONDEFENSE_GAME_H