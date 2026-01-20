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

	m_enemyPool.forEachActive([&](Enemy& e)
	{
		e.update(deltaMilliseconds);

		if (e.isTargetReached() /* or e.isDead() or out of bounds */)
		{
			m_enemyPool.release(&e);
		}
	});
}

void World::render(sf::RenderWindow& window)
{
	// Nothing to render for now
	// (You can keep window.clear() in main.cpp)

	if (m_station) m_station->render(window);

	m_enemyPool.forEachActive([&](Enemy& e)
	{
		e.render(window); // if you have render per entity
	});
}

void World::setStation(std::unique_ptr<Station> station)
{
	m_station = std::move(station);
}

void World::spawnEnemy(const Enemy::EnemyDescriptor& baseDesc,
					   const sf::Vector2f targetPos,
					   const float targetSize)
{
	for (int i = 0; i < 10; ++i)
	{
		Enemy::EnemyDescriptor desc = baseDesc; // copia LOCAL

		desc.position = {
			static_cast<float>(std::rand() % SCREEN_WIDTH),
			static_cast<float>(std::rand() % SCREEN_HEIGHT)
		};

		const Enemy* enemy = m_enemyPool.acquire([&](Enemy& e)
		{
			e.init(desc);
			e.setTarget(targetPos, targetSize);
		});

		if (!enemy)
			break; // pool lleno
	}
}

const Station* World::getStation() const
{
	return m_station.get();
}
