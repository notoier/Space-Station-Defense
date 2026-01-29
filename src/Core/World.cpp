// src/Core/World.cpp
#include "Core/World.h"

#include <random>

#include "Utils/Constants.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "Gameplay/Cannon.h"
#include "Gameplay/Laser.h"
#include "SFML/System/Sleep.hpp"

bool World::load()
{
	if (m_station)
	{
		for (auto& w : m_station->getWeapons())
		{

			w->setSpawnProjectileFn([this](const Projectile::ProjectileDescriptor& d)

			{
				m_projectilePool.acquire([&](Projectile& p)
				{
					p.init(d);
				});
			});
		}
	}

	return true;
}

void World::update(const float deltaMilliseconds)
{
	const float dtSeconds = deltaMilliseconds / 1000.f;

	if (m_station)
	{
		Weapon::WeaponContext ctx;
		ctx.originWorld = m_station->getCenter();
		ctx.aimWorld    = m_station->getAimWorld();

		m_station->update(deltaMilliseconds);
		for (auto& w : m_station->getWeapons())
		{
			w->setWeaponContext(ctx);
			w->update(dtSeconds);
			w->applyEffectToEnemies(m_enemyPool);
		}
	}


	m_projectilePool.forEachActive([&](Projectile& p)
	{
		p.update(dtSeconds);
		if (!p.isAlive())
			m_projectilePool.release(&p);
	});

	m_enemyPool.forEachActive([&](Enemy& e)
	{
		e.update(deltaMilliseconds);

		if (e.isTargetReached())
		{
			m_enemyPool.release(&e);
			m_station->receiveDamage(e.getDamage());
			m_onHealthDamageReceived(m_station->getHealth() / m_station->getMaxHealth());
		}

		else if (!e.isAlive())
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
	}

	m_enemyPool.forEachActive([&](Enemy& e)
	{
		e.render(window); // if you have render per entity
	});

	m_projectilePool.forEachActive([&](Projectile& p)
	{
		p.render(window);
	});
}

void World::setStation(std::unique_ptr<Station> station)
{
	m_station = std::move(station);
}

void World::spawnEnemy(const sf::Vector2f targetPos, const float targetSize)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.1, 0.25);
	std::uniform_real_distribution<double> dist2(0.75, 0.9);
	std::uniform_int_distribution<int> side(0, 1);

	// If no more waves defined, then repeat last wave
	const int currentWave = m_currentWave < waves.size() ? m_currentWave : static_cast<int>(waves.size() - 1);
	for (int i = 0; i < waves[currentWave].amountOfEnemies; ++i)
	{
		Enemy::EnemyDescriptor desc = waves[currentWave].descriptor;

		desc.position = {
			static_cast<float>((side(mt) ? dist(mt) : dist2(mt))) * SCREEN_WIDTH,
			static_cast<float>((side(mt) ? dist(mt) : dist2(mt))) * SCREEN_HEIGHT
		};

		static uint32_t g_enemyUID = 1;

		const Enemy* enemy = m_enemyPool.acquire([&](Enemy& e)
		{
			e.init(desc);
			e.setTarget(targetPos, targetSize);
			e.setUID(g_enemyUID++);
		});

		if (!enemy)
			break;// pool lleno

		m_amountOfEnemies++;
	}

	m_currentWave++;
}

const Station* World::getStation() const
{
	return m_station.get();
}

void World::setOnDamageFunction(const std::function<void(float damage)>& func)
{
	m_onHealthDamageReceived = func;
}

void World::setAimWorld(const sf::Vector2f& aimWorld)
{
	if (m_station)
		m_station->setAimWorld(aimWorld);
}

void World::addWave(const Wave& wave)
{
	waves.push_back(wave);
}

void World::onLeftClick()
{
	if (m_station)
	{
		m_station->onLeftClick(m_enemyPool);
	}

}
