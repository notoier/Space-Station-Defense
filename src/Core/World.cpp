// src/Core/World.cpp
// ReSharper disable CppDFAConstantParameter
#include "Core/World.h"

#include <iostream>
#include <random>

#include "Utils/Constants.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "Gameplay/Cannon.h"
#include "Gameplay/Laser.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool World::load()
{
	/* Reset */
	m_currentWave = 0;
	m_waveActive = false;
	m_amountOfEnemies = 0;

	/* Station & Weapons Config */
	if (!loadStationFromJson("data/config/station.json"))
	{
		std::cout << "[World] Failed to load station.json. Using fallback.\n";

		/* Default */
		Station::StationDescriptor stationDesc;
		stationDesc.position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
		stationDesc.health = 20.f;
		stationDesc.visualType = Entity::VisualType::StationCircle;
		stationDesc.radius = 50.f;

		auto station = std::make_unique<Station>();
		station->init(stationDesc);

		Laser::LaserBaseStats laserStats;
		laserStats.range = 600.f;
		laserStats.damage = 0.5f;
		laserStats.width = 25.f;
		station->addWeapon(std::make_unique<Laser>(laserStats));

		Cannon::CannonDesc cannonStats;
		cannonStats.damage = 0.5f;
		cannonStats.fireCooldownSec = 1.f;
		cannonStats.orbitRadius = 75.f;
		cannonStats.targetMaxDistance = 10000.f;
		cannonStats.orbitSpeedDegPerSec = 180.f;
		cannonStats.projectileSpeed = 150.f;
		cannonStats.projectileLifeSec = 8.f;
		station->addWeapon(std::make_unique<Cannon>(cannonStats));

		setStation(std::move(station));
		setTarget(stationDesc.position, stationDesc.radius);
	}

	/* Weapon callbacks */
	bindWeaponCallbacks();


	/* Load Waves */
	if (!loadWavesFromJson("data/config/waves.json"))
	{
		std::cout << "[World] Failed to load waves.json. Using fallback.\n";

		waves.clear();
		Wave w{};
		w.amountOfEnemies = 10;
		w.descriptor.health = 3.f;
		w.descriptor.visualType = Entity::VisualType::BasicEnemySquares;
		w.descriptor.velocity = { 120.f, 0.f };
		w.descriptor.size = 28.f;
		w.descriptor.damage = 1.f;
		waves.push_back(w);
	}

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


// ReSharper disable once CppDFAConstantParameter
static int getIntOr(const json& j, const char* key, const int def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_number_integer()) return def;
    return j[key].get<int>();
}

static float getFloatOr(const json& j, const char* key, const float def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_number()) return def;
    return j[key].get<float>();
}

static std::string getStringOr(const json& j, const char* key, const std::string& def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_string()) return def;
    return j[key].get<std::string>();
}

static sf::Vector2f getVectorOr(const json& j, const char* key, sf::Vector2f& def)
{
	if (!j.contains(key) || !j[key].is_object())
		return def;

	const json& v = j[key];
	def.x = getFloatOr(v, "x", def.x);
	def.y = getFloatOr(v, "y", def.y);
	return def;
}

bool World::loadWavesFromJson(const std::string& filePath)
{
    std::ifstream wavesFile(filePath);
    if (!wavesFile.is_open())
    {
        std::cout << "[World] Could not open: " << filePath << "\n";
        return false;
    }

    json root;
    try
    {
        wavesFile >> root;
    }

	catch (const std::exception& e)
    {
        std::cout << "[World] JSON parse error: " << e.what() << "\n";
        return false;
    }

    if (!root.contains("waves") || !root["waves"].is_array())
    {
        std::cout << "[World] Invalid format: missing 'waves' array.\n";
        return false;
    }

    waves.clear();

    for (const auto& wj : root["waves"])
    {
        if (!wj.is_object())
            continue;

        Wave wave{};
        wave.amountOfEnemies = std::max(1, getIntOr(wj, "amount", 1));

        /* Init Default */
        wave.descriptor.health = 1.f;
        wave.descriptor.visualType = Entity::VisualType::BasicEnemySquares;
        wave.descriptor.velocity = { 50.f, 0.f };
        wave.descriptor.size = 28.f;
        wave.descriptor.damage = 1.f;

        if (wj.contains("enemy") && wj["enemy"].is_object())
        {
            const json& ej = wj["enemy"];

            const float speed = getFloatOr(ej, "speed", 120.f);

            wave.descriptor.health = getFloatOr(ej, "health", 3.f);
            wave.descriptor.size   = getFloatOr(ej, "size", 28.f);
            wave.descriptor.damage = getFloatOr(ej, "damage", 1.f);

            // Store speed in velocity; real direction will be handled by setTarget() or enemy logic
            wave.descriptor.velocity = { speed, 0.f };

            // optional visualType
            if (ej.contains("visualType"))
            {
                const std::string visualType = getStringOr(ej, "visualType", "");
                if (!visualType.empty())
                    wave.descriptor.visualType = Entity::parseVisualType(visualType, wave.descriptor.visualType);
            }
        }

        waves.push_back(wave);
    }

    std::cout << "[World] Loaded " << waves.size() << " waves.\n";
    return !waves.empty();
}

bool World::loadStationFromJson(const std::string& path)
{
	std::ifstream f(path);
    if (!f.is_open())
    {
        std::cout << "[World] Could not open: " << path << "\n";
        return false;
    }

    json root;
    try
    {
        f >> root;
    }
    catch (const std::exception& e)
    {
        std::cout << "[World] station.json parse error: " << e.what() << "\n";
        return false;
    }

    if (!root.contains("station") || !root["station"].is_object())
    {
        std::cout << "[World] Invalid station.json: missing 'station' object.\n";
        return false;
    }

    const json& sj = root["station"];

    Station::StationDescriptor stationDesc;
	sf::Vector2f pos = {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f};
    stationDesc.position = getVectorOr(sj, "position", pos);
    stationDesc.health = getFloatOr(sj, "health", 20.f);
    stationDesc.radius = getFloatOr(sj, "radius", 50.f);

    const std::string vt = getStringOr(sj, "visualType", "StationCircle");
    stationDesc.visualType = Entity::parseVisualType(vt, Entity::VisualType::StationCircle);

    auto station = std::make_unique<Station>();
    station->init(stationDesc);

    // Weapons
    if (root.contains("weapons") && root["weapons"].is_array())
    {
        for (const auto& wj : root["weapons"])
        {
            if (!wj.is_object())
                continue;

            const std::string type = getStringOr(wj, "type", "");
            if (type.empty())
                continue;

            const json params = wj.contains("params") && wj["params"].is_object() ? wj["params"] : json::object();

            if (type == "Laser")
            {
                Laser::LaserBaseStats s;
                s.range  = getFloatOr(params, "range", 600.f);
                s.damage = getFloatOr(params, "damage", 0.5f);
                s.width  = getFloatOr(params, "width", 25.f);

                station->addWeapon(std::make_unique<Laser>(s));
            }
            else if (type == "Cannon")
            {
                Cannon::CannonDesc s;
                s.damage             = getFloatOr(params, "damage", 0.5f);
                s.fireCooldownSec    = getFloatOr(params, "fireCooldownSec", 1.f);
                s.orbitRadius        = getFloatOr(params, "orbitRadius", 75.f);
                s.targetMaxDistance  = getFloatOr(params, "targetMaxDistance", 10000.f);
                s.orbitSpeedDegPerSec= getFloatOr(params, "orbitSpeedDegPerSec", 180.f);
                s.projectileSpeed    = getFloatOr(params, "projectileSpeed", 150.f);
                s.projectileLifeSec  = getFloatOr(params, "projectileLifeSec", 8.f);

                station->addWeapon(std::make_unique<Cannon>(s));
            }
            else
            {
                std::cout << "[World] Unknown weapon type: " << type << "\n";
            }
        }
    }

    setTarget(stationDesc.position, stationDesc.radius);
    setStation(std::move(station));
    return true;
}

void World::bindWeaponCallbacks()
{
	if (!m_station)
		return;

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
		});

		if (!enemy)
		{
			std::cout << "Pool llena" << std::endl;
			break;
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

void World::setAimWorld(const sf::Vector2f& aimWorld) const
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
			std::cout<<"Laser Upgrades"<< std::endl;
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
		case UpgradeId::Laser_Damage: return 0;
		case UpgradeId::Laser_Range:  return 0;
		case UpgradeId::Cannon_Damage: return 0;
		case UpgradeId::Cannon_FireRate: return 0;
		// ToDo: Barrier
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