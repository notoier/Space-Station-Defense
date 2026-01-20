// src/Core/World.cpp
#include "Core/World.h"
#include "Utils/Constants.h"
#include <SFML/Graphics/RenderWindow.hpp>

bool World::load()
{
	// Nothing to load for now
	return true;
}

void World::update(const float deltaMilliseconds)
{
	if (m_station) m_station->update(deltaMilliseconds);

	for (const auto& entity : m_entities)
	{
		if (entity->isAlive())
		{
			entity->update(deltaMilliseconds);
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	// Nothing to render for now
	// (You can keep window.clear() in main.cpp)

	if (m_station) m_station->render(window);

	for (const auto& entity : m_entities)
	{
			entity->render(window);
	}
}

void World::addEntity(std::unique_ptr<Entity> entity)
{
	m_entities.push_back(std::move(entity));
}

void World::setStation(std::unique_ptr<Station> station)
{
	m_station = std::move(station);
}

const Station* World::getStation() const
{
	return m_station.get();
}
