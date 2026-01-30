// src/Core/World.cpp
#include "Core/World.h"

#include <iostream>
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

	// 1) Station + weapons
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

	// 2) Enemies update + release
	std::vector<Enemy*> toRelease;
	toRelease.reserve(32);

	m_enemyPool.forEachActive([&](Enemy& e)
	{
	    e.update(deltaMilliseconds);

	    if (e.isTargetReached())
	    {
	        m_station->receiveDamage(e.getDamage());
	        e.receiveDamage(e.getHealth());
	        if (m_onHealthDamageReceived)
	    		m_onHealthDamageReceived(m_station->getHealth() / m_station->getMaxHealth());

	    	toRelease.push_back(&e);
	        return;
	    }

	    if (!e.isAlive())
	    {
	        toRelease.push_back(&e);
	        return;
	    }
	});

	for (Enemy* e : toRelease)
	    m_enemyPool.release(e);

	// 3) Projectiles update + release
	m_projectilePool.forEachActive([&](Projectile& p)
	{
		p.update(dtSeconds);

		if (!p.isAlive())
		{
			m_projectilePool.release(&p);
			return;
		}
	});

	// 4) Collisions (projectile vs enemy)
	m_projectilePool.forEachActive([&](Projectile& p)
	{
		if (!p.isAlive())
			return;

		const sf::FloatRect pb = p.getBounds();

		m_enemyPool.forEachActive([&](Enemy& e)
		{
			if (!p.isAlive() || !e.isAlive())
				return;

			if (pb.intersects(e.getBounds()))
			{
				e.receiveDamage(p.getDamage());
				p.kill();
			}
		});
	});

	if (!m_waveActive)
	{
		spawnEnemy();
	}
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

void World::spawnEnemy()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.1, 0.25);
	std::uniform_real_distribution<double> dist2(0.75, 0.9);
	std::uniform_int_distribution<int> side(0, 1);

	// If no more waves defined, then repeat last wave
	const int currentWave = m_currentWave < waves.size() ? m_currentWave : static_cast<int>(waves.size() - 1);
	std::cout << "Current wave: " << currentWave << std::endl;
	std::cout << "Amount (wave): "<< waves[currentWave].amountOfEnemies << std::endl;

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
			e.setTarget(m_station_pos, m_station_size);
			e.setUID(g_enemyUID++);
			e.setOnDeathFunction([this]() {onEnemyDeath(); });

			std::cout << "M_Amount: " << m_amountOfEnemies << std::endl;
		});

		if (!enemy)
		{
			std::cout << "Pool llena" << std::endl;
			break;// pool lleno
		}
		m_amountOfEnemies++;
	}

	m_waveActive = true;
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

void World::setOnEnemyDeathFunction(const std::function<void(int currency)>& func)
{
	m_onEnemyDeath = func;
}

void World::setTarget(const sf::Vector2f targetPos, const float targetSize)
{
	m_station_pos = targetPos;
	m_station_size = targetSize;
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

void World::addCurrency(const int money)
{
	m_currency += money;
}

void World::subCurrency(const int money)
{
	m_currency -= money;
}

int World::getCurrency() const
{
	return m_currency;
}

void World::onEnemyDeath()
{
	m_currency++;
	m_amountOfEnemies--;
	if (m_amountOfEnemies <= 0){
		m_waveActive = false; // New wave
	}

	m_onEnemyDeath(m_currency);
}

void World::onLeftClick()
{
	if (m_station)
	{
		m_station->onLeftClick(m_enemyPool);
	}

}

void World::applyUpgrade(const UpgradeId id)
{
	switch (id)
	{
		case UpgradeId::Laser_Damage:
			m_laserUpgrades.damageUpgrade += 0.5f;
			break;

		case UpgradeId::Laser_Range:
			m_laserUpgrades.rangeUpgrade += 50.f;
			break;

		case UpgradeId::Cannon_Damage:
			m_cannonUpgrades.damageUpgrade += 0.5f;

		case UpgradeId::Cannon_FireRate:
			m_cannonUpgrades.cooldownReduction += 0.15f;

		// ToDO: Barrier

		default:
			break;
	}

	if (m_station)
	{
		for (auto& w : m_station->getWeapons())
		{
			if (auto* laser = dynamic_cast<Laser*>(w.get()))
				laser->setUpgrades(m_laserUpgrades);
			else if (auto* cannon = dynamic_cast<Cannon*>(w.get()))
				cannon->setUpgrades(m_cannonUpgrades);
			// ToDo: Barrier upgrades
		}
	}
}

int World::getUpgradeCost(const UpgradeId id) const
{
	switch (id)
	{
	case UpgradeId::Laser_Damage: return 50;
	case UpgradeId::Laser_Range:  return 75;
	default: return 999999;
	}
}

bool World::tryBuyUpgrade(const UpgradeId id)
{
	const int cost = getUpgradeCost(id);
	if (m_currency < cost)
		return false;

	m_currency -= cost;
	applyUpgrade(id);

	// m_onCurrencyChanged(m_currency);

	return true;
}
