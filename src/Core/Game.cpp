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

#include "Utils/Constants.h"

bool Game::init(GameCreateInfo& createInfo)
{
    assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

    m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
    m_window->setFramerateLimit(createInfo.frameRateLimit);

    // unique_ptr owns the World
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
}

bool Game::isRunning() const
{
    return m_window && m_window->isOpen();
}

void Game::update(uint32_t deltaMilliseconds)
{
    // Check if user closed the window
    for (auto event = sf::Event(); m_window->pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            std::cout << event.mouseButton.button << std::endl;
            m_world->onLeftClick();
        }
    }

    const sf::Vector2i mousePx = sf::Mouse::getPosition(*m_window);
    const sf::Vector2f mouseWorld = m_window->mapPixelToCoords(mousePx);

    if (m_world)
    {
        m_world->setAimWorld(mouseWorld);

        // Update scene here
        m_world->update(static_cast<float>(deltaMilliseconds));
    }
}

void Game::render()
{
    m_window->clear(BACKGROUND_COLOR);
    m_world->render(*m_window);
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
