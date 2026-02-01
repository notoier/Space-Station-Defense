//
// Created by Oier Álvarez on 18/01/2026.
//

#include "Core/Game.h"
#include "Core/World.h"

#include <cassert>
#include <iostream>
#include <ostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "UI/PauseMenu.h"
#include "Utils/Constants.h"

bool Game::init(GameCreateInfo& createInfo)
{
    /* Regular window */
    assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

    m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
    m_window->setFramerateLimit(createInfo.frameRateLimit);

    /* Pause Menu */
    m_pauseWindow = new PauseMenu();
    m_pauseWindow->setResumeFunc([this]() {resumeGame();});
    m_pauseWindow->setRetryFunc([this]() {retryGame();});
    m_pauseWindow->setExitFunc([this]() {quitGame();});

    m_pauseOverlay.setSize({
       static_cast<float>(createInfo.screenWidth),
       static_cast<float>(createInfo.screenHeight)
   });

    m_pauseOverlay.setFillColor(PAUSED_BACKGROUND_COLOR);

    /* Regular UI */
    m_ui = new UI();
    m_ui->init();

    /* World */
    m_world = std::make_unique<World>();
    m_world -> setOnDamageFunction([this](const float healthPercentage) {damageReceived(healthPercentage);});
    m_world -> setOnBarrierDamageFunction([this](const float healthPercentage) {barrierDamageReceived(healthPercentage);});
    m_world -> setOnBarrierHealthGainedFunction([this](const float healthPercentage) {barrierHealthGained(healthPercentage);});
    m_world -> setOnCurrencyUpdateFunction([this](const int currency) {updateCurrency(currency);});

    /* Upgrade  menu */
    m_upgradeWindow = new UpgradeMenu();
    m_upgradeWindow->init();

    m_upgradeWindow->setOnUpgradeSelected([this](const UpgradeId id)
    {
        if (!m_world) return;

        if (m_world->tryBuyUpgrade(id))
        {
            m_upgradeWindow->refreshTexts();
        }
    });

    m_upgradeWindow->setGetUpgradeTextFn([this](const UpgradeId id) -> std::string
    {
        if (!m_world) return "???";

     const std::string name = m_world->getUpgradeName(id);
     const int cost = m_world->getUpgradeCost(id);
     const int level = m_world->getUpgradeLevel(id);
     const int maxLevel = m_world->getUpgradeMaxLevel(id);

     if (level >= maxLevel)
         return name + "\nMAX";

     return name + "\nCost: " + std::to_string(cost) + "  Lv: " + std::to_string(level);
    });

    m_upgradeWindow->refreshTexts();

    const bool loadOk = m_world->load();

    return loadOk;
}

Game::~Game()
{
    // To-Do: make sure m_world is unloaded()

    m_world.reset();

    delete m_window;
    m_window = nullptr;

    delete m_pauseWindow;
    m_pauseWindow = nullptr;
}

bool Game::isRunning() const
{
    return m_window && m_window->isOpen();
}

void Game::update(uint32_t deltaMilliseconds)
{
    for (auto event = sf::Event(); m_window->pollEvent(event);)
    {
        // Check if user closed the window
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
        }

        if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::P || event.key.code == sf::Keyboard::Escape))
        {
            togglePause();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
        {
            toggleUpgradeMenu();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                const sf::Vector2i mousePx = sf::Mouse::getPosition(*m_window);
                const sf::Vector2f mouseWorld = m_window->mapPixelToCoords(mousePx);

                /* Click on pause menu */
                if (m_isPaused)
                {
                    m_pauseWindow->onLeftClick(mouseWorld);
                    continue;
                }

                /* Click on upgrades menu */
                if (m_upgradesOpened)
                {
                    m_upgradeWindow->onLeftClick(mouseWorld);
                    continue;
                }

                m_world->onLeftClick();
            }
        }

    }

    if (m_isPaused) return;

    const sf::Vector2i mousePx = sf::Mouse::getPosition(*m_window);
    const sf::Vector2f mouseWorld = m_window->mapPixelToCoords(mousePx);

    if (m_world)
    {
        m_world->setAimWorld(mouseWorld);
        m_world->update(static_cast<float>(deltaMilliseconds));
    }
}

void Game::render()
{
    m_window->clear(BACKGROUND_COLOR);
    m_world->render(*m_window);
    m_ui->render(*m_window);

    // Dark overlay
    if (m_isPaused || m_upgradesOpened)
        m_window->draw(m_pauseOverlay);

    if (m_pauseWindow && m_pauseWindow->isEnabled())
    {
        m_pauseWindow->render(*m_window);
    }

    else if (m_upgradesOpened && m_upgradeWindow->isEnabled())
    {
        m_upgradeWindow->render(*m_window);
    }

    m_window->display();

}

World* Game::getWorld() const
{
    return m_world.get();
}

sf::RenderWindow& Game::getWindow()
{
    // Assume m_window is valid after init()
    return *m_window;
}

const sf::RenderWindow& Game::getWindow() const
{
    return *m_window;
}

void Game::restartGame()
{
    if (!m_world) return;

    /* Reset pools & weapons */
    m_world->resetGame();

    /* Reset UI */
    m_isPaused = false;
    if (m_pauseWindow) m_pauseWindow->enable(false);

    m_upgradesOpened = false;
    if (m_upgradeWindow)
    {
        m_upgradeWindow->refreshTexts();
        m_upgradeWindow->enable(false);
    }
        if (m_ui) m_ui->reset();

    /* Reload */
    m_world->load();
}

void Game::pauseGame()
{
    closeUpgradeMenu();
    m_isPaused = true;
    if (m_pauseWindow) m_pauseWindow->enable(true);
}

void Game::resumeGame()
{
    m_isPaused = false;
    if (m_pauseWindow) m_pauseWindow->enable(false);
}

void Game::togglePause()
{
    m_isPaused ? resumeGame() : pauseGame();
}

void Game::toggleUpgradeMenu()
{
    m_upgradesOpened ? closeUpgradeMenu() : openUpgradeMenu();
}

void Game::closeUpgradeMenu()
{
    m_upgradesOpened = false;
    if (m_upgradeWindow) m_upgradeWindow->enable(false);
}

void Game::openUpgradeMenu()
{
    resumeGame();
    m_upgradesOpened = true;
    if (m_upgradeWindow)
    {
        m_upgradeWindow->enable(true);
        m_upgradeWindow->refreshTexts();
    }
}

void Game::quitGame()
{
    m_window->close();
}

void Game::retryGame()
{
    std::cout << "Opening Settings..." << std::endl;
    restartGame();
}

void Game::damageReceived(const float healthPercentage) const
{
    m_ui->healthDown(healthPercentage);
}

void Game::barrierDamageReceived(const float healthPercentage) const
{
    m_ui->barrierDown(healthPercentage);
}

void Game::barrierHealthGained(const float healthPercentage) const
{
    m_ui->barrierUp(healthPercentage);
}

void Game::updateCurrency(const int currency)
{
    m_ui->updateCurrency(currency);
}
