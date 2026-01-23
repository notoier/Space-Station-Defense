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
	const float dtSeconds = deltaMilliseconds / 1000.f;

	if (m_station)
	{
		m_station->update(deltaMilliseconds);

		Weapon::WeaponContext ctx;
		ctx.originWorld = m_station->getCenter();
		ctx.aimWorld    = m_station->getAimWorld();

		for (auto& w : m_station->getWeapons())
		{
			w->setWeaponContext(ctx);
			w->update(dtSeconds);
		}

	}

	m_enemyPool.forEachActive([&](Enemy& e)
	{
		e.update(deltaMilliseconds);

		if (e.isTargetReached() || !e.isAlive())
		{
			m_enemyPool.release(&e);
		}
	});
}

void World::render(sf::RenderWindow& window)
{
	// Nothing to render for now
	// (You can keep window.clear() in main.cpp)

	if (m_station)
	{
		m_station->render(window);
		for (auto& w : m_station->getWeapons())
		{
			w->render(window);
		}
	}
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

void World::setAimWorld(const sf::Vector2f& aimWorld)
{
	if (m_station)
		m_station->setAimWorld(aimWorld);
}

void World::onLeftClick()
{
	if (m_station)
	{
		m_station->onLeftClick();
		for (auto& w : m_station->getWeapons())
		{
			w->applyEffectToEnemies(m_enemyPool);
		}
	}

}
