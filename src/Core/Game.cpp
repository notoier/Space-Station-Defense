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
    assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

    m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
    m_window->setFramerateLimit(createInfo.frameRateLimit);

    m_pauseWindow = new PauseMenu();
    m_pauseWindow->setResumeFunc([this]() {resumeGame();});
    m_pauseWindow->setSettingsFunc([this]() {openSettings();});
    m_pauseWindow->setExitFunc([this]() {quitGame();});

    m_pauseOverlay.setSize({
       static_cast<float>(createInfo.screenWidth),
       static_cast<float>(createInfo.screenHeight)
   });

    m_pauseOverlay.setFillColor(PAUSED_BACKGROUND_COLOR);

    m_world = std::make_unique<World>();
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

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
        {
            togglePause();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (m_isPaused)
                {
                    const sf::Vector2i mousePx = sf::Mouse::getPosition(*m_window);
                    const sf::Vector2f mouseWorld = m_window->mapPixelToCoords(mousePx);
                    m_pauseWindow->onLeftClick(mouseWorld);

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

    // Dark overlay
    if (m_isPaused)
        m_window->draw(m_pauseOverlay);

    if (m_pauseWindow && m_pauseWindow->isEnabled())
    {
        m_pauseWindow->render(*m_window);
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

void Game::pauseGame()
{
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

void Game::quitGame()
{
    std::cout << "Quitting Game..." << std::endl;
}

void Game::openSettings()
{
    std::cout << "Opening Settings..." << std::endl;
}
