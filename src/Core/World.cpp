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

#include "Utils/MathUtils.h"
#include <cmath>

using json = nlohmann::json;

bool World::load()
{
	/* Reset */
	m_currentWave = 0;
	m_waveActive = false;
	m_amountOfEnemies = 0;

	/* Station & Weapons Config */
	if (!loadStationAndWeaponsFromJson("data/config/station.json"))
	{
		std::cout << "[World] Failed to load station.json. Using fallback.\n";

		/* Default values for station and weapons in case it fails */
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
		/* Default values for waves in case it fails */
		std::cout << "[World] Failed to load waves.json. Using fallback.\n";
		m_waves.clear();
		for (int i = 0; i < 10; i++)
		{
			Wave w{};
			w.amountOfEnemies = 10;
			w.descriptor.health = 3.f;
			w.descriptor.visualType = Entity::VisualType::BasicEnemySquares;
			w.descriptor.velocity = { 120.f, 0.f };
			w.descriptor.size = 28.f;
			w.descriptor.damage = 1.f;
			m_waves.push_back(w);
		}
	}

	/* Load Upgrades */
	if (!loadUpgradesFromJson("data/config/upgrades.json"))
	{
		/* Default values for upgrades in case it fails */
		std::cout << "[World] Failed to load upgrades.json\n";

		m_upgradeDefs.clear();
		m_upgradeState.clear();

		// ===== LASER =====
		m_upgradeDefs[UpgradeId::Laser_Damage] =
		{
			"Laser Damage",   // uiName
			50,               // baseCost
			1.25f,            // costScale
			10,               // maxLevel
			0.5f              // delta
		};
		m_upgradeState[UpgradeId::Laser_Damage] = {};

		m_upgradeDefs[UpgradeId::Laser_Range] =
		{
			"Laser Range",
			75,
			1.20f,
			8,
			50.f
		};
		m_upgradeState[UpgradeId::Laser_Range] = {};

		// ===== BARRIER =====
		m_upgradeDefs[UpgradeId::Barrier_MaxHealth] =
		{
			"Barrier Max HP",
			60,
			1.25f,
			10,
			10.f
		};
		m_upgradeState[UpgradeId::Barrier_MaxHealth] = {};

		m_upgradeDefs[UpgradeId::Barrier_RegenAmount] =
		{
			"Barrier Regen",
			70,
			1.22f,
			8,
			2.f
		};
		m_upgradeState[UpgradeId::Barrier_RegenAmount] = {};

		// ===== CANNON =====
		m_upgradeDefs[UpgradeId::Cannon_Damage] =
		{
			"Cannon Damage",
			60,
			1.25f,
			10,
			0.5f
		};
		m_upgradeState[UpgradeId::Cannon_Damage] = {};

		m_upgradeDefs[UpgradeId::Cannon_FireRate] =
		{
			"Cannon Fire Rate",
			80,
			1.30f,
			6,
			0.15f
		};
		m_upgradeState[UpgradeId::Cannon_FireRate] = {};

		std::cout << "[World] Fallback upgrades initialized ("
				  << m_upgradeDefs.size() << " upgrades).\n";
	}

	return true;
}

void World::resetGame()
{
	/* Reset waves */
	m_currentWave = 0;
	m_waveActive = false;
	m_amountOfEnemies = 0;
	m_waves = {};

	/* Reset currency */
	m_currency = 0;
	if (m_onCurrencyUpdate)
		m_onCurrencyUpdate(m_currency);

	/* Reset enemy pool */
	m_enemyPool.forEachActive([&](Enemy& e)
	{
		m_enemyPool.release(&e);
	});

	/* Reset projectile pool */
	m_projectilePool.forEachActive([&](Projectile& p)
	{
		m_projectilePool.release(&p);
	});

	/* Reset upgrades (levels) + reset upgrade deltas */
	for (auto& [id, st] : m_upgradeState)
		st.level = 0;

	m_laserUpgrades = {};
	m_cannonUpgrades = {};
	m_barrierUpgrades = {};

	/* Re-apply upgrades to weapons */
	if (m_station)
	{
		for (auto& w : m_station->getWeapons())
		{
			if (auto* laser = dynamic_cast<Laser*>(w.get()))
				laser->setUpgrades(m_laserUpgrades);
			else if (auto* cannon = dynamic_cast<Cannon*>(w.get()))
				cannon->setUpgrades(m_cannonUpgrades);
			else if (auto* barrier = dynamic_cast<Barrier*>(w.get()))
				barrier->setUpgrades(m_barrierUpgrades);
		}

		/* Reset station health */
		m_station->reset();
	}
}

// ReSharper disable once CppDFAConstantParameter
/* Get int or default value from json */
static int getIntOr(const json& j, const char* key, const int def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_number_integer()) return def;
    return j[key].get<int>();
}

/* Get float or default value from json */
static float getFloatOr(const json& j, const char* key, const float def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_number()) return def;
    return j[key].get<float>();
}

/* Get string or default value from json */
static std::string getStringOr(const json& j, const char* key, const std::string& def)
{
    if (!j.contains(key)) return def;
    if (!j[key].is_string()) return def;
    return j[key].get<std::string>();
}

/* Get vector or default value from json */
static sf::Vector2f getVectorOr(const json& j, const char* key, sf::Vector2f& def)
{
	if (!j.contains(key) || !j[key].is_object())
		return def;

	const json& v = j[key];
	def.x = getFloatOr(v, "x", def.x);
	def.y = getFloatOr(v, "y", def.y);
	return def;
}

static bool tryParseUpgradeId(const std::string& s, UpgradeId& out)
{
	if (s == "Laser_Damage")         { out = UpgradeId::Laser_Damage; return true; }
	if (s == "Laser_Range")          { out = UpgradeId::Laser_Range; return true; }

	if (s == "Barrier_MaxHealth")    { out = UpgradeId::Barrier_MaxHealth; return true; }
	if (s == "Barrier_RegenAmount")  { out = UpgradeId::Barrier_RegenAmount; return true; }

	if (s == "Cannon_Damage")        { out = UpgradeId::Cannon_Damage; return true; }
	if (s == "Cannon_FireRate")      { out = UpgradeId::Cannon_FireRate; return true; }

	return false;
}

/* Upgrades from upgrades.json */
bool World::loadUpgradesFromJson(const std::string& path)
{
	std::ifstream f(path);
	if (!f.is_open())
	{
		std::cout << "[World] Could not open: " << path << "\n";
		return false;
	}

	json root;
	try { f >> root; }
	catch (const std::exception& e)
	{
		std::cout << "[World] upgrades.json parse error: " << e.what() << "\n";
		return false;
	}

	if (!root.contains("upgrades") || !root["upgrades"].is_array())
	{
		std::cout << "[World] Invalid upgrades.json: missing 'upgrades' array\n";
		return false;
	}

	m_upgradeDefs.clear();
	m_upgradeState.clear();

	for (const auto& uj : root["upgrades"])
	{
		if (!uj.is_object())
			continue;

		const std::string idStr = getStringOr(uj, "id", "");
		UpgradeId id{};
		if (!tryParseUpgradeId(idStr, id))
		{
			std::cout << "[World] Unknown upgrade id: " << idStr << "\n";
			continue;
		}

		UpgradeDefinition def{};
		def.uiName    = getStringOr(uj, "uiName", idStr);
		def.baseCost  = getIntOr(uj, "baseCost", 0);
		def.costScale = getFloatOr(uj, "costScale", 1.f);
		def.maxLevel  = getIntOr(uj, "maxLevel", 1);
		def.delta     = getFloatOr(uj, "delta", 0.f);

		m_upgradeDefs[id] = def;
		m_upgradeState[id] = UpgradeState{}; // level = 0
	}

	std::cout << "[World] Loaded " << m_upgradeDefs.size() << " upgrades.\n";
	return !m_upgradeDefs.empty();
}

/* Waves from waves.json */
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

    m_waves.clear();

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

        m_waves.push_back(wave);
    }

    std::cout << "[World] Loaded " << m_waves.size() << " waves.\n";
    return !m_waves.empty();
}

/* Station & Weapons from station.json */
bool World::loadStationAndWeaponsFromJson(const std::string& path)
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
			else if (type == "Barrier")
        	{
				Barrier::BarrierDesc s;
				s.regenCooldown  = getFloatOr(params, "regenCooldown", 10.f);
				s.regenAmount    = getFloatOr(params, "regenAmount", 1.f);
        		s.barrierHealth  = getFloatOr(params, "barrierHealth", 10.f);

				station->addWeapon(std::make_unique<Barrier>(s));
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

	m_station->setGameOverFunction(m_onGameOver);

	for (auto& w : m_station->getWeapons())
	{
		if (auto* b = dynamic_cast<Barrier*>(w.get()))
		{
			m_barrier = b;
			m_barrier->setOnBarrierHealthGainedFunction(m_onBarrierHealthGained);
		}

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
		ctx.originWorld = STATION_POSITION;
		ctx.aimWorld    = m_station->getAimWorld();

		m_station->update(deltaMilliseconds);

		for (const auto& w : m_station->getWeapons())
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

		bool barrierColision = false;
		if (m_barrier->isEnabled())
		{
			barrierColision = BoxCircumferenceCollision(e.getPosition(), e.getSize(), STATION_POSITION, BARRIER_SIZE);
		}

	    if (e.isTargetReached() || barrierColision)
	    {
			if (m_barrier && m_barrier->getHealth() > 0.f)
	        {
				const float remainingDamage = m_barrier->receiveDamage(e.getDamage());
	        	if (m_onBarrierDamageReceived)
	        		m_onBarrierDamageReceived(m_barrier->getHealth() / m_barrier->getMaxHealth());

	        	m_station->receiveDamage(remainingDamage);
	        }
			else
	        {
	        	m_station->receiveDamage(e.getDamage());
	        }
			if (m_onHealthDamageReceived)
	    		m_onHealthDamageReceived(m_station->getHealth() / m_station->getMaxHealth());


	    	e.receiveDamage(e.getHealth());

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
	const int currentWave = m_currentWave < m_waves.size() ? m_currentWave : static_cast<int>(m_waves.size() - 1);
	for (int i = 0; i < m_waves[currentWave].amountOfEnemies; ++i)
	{
		Enemy::EnemyDescriptor desc = m_waves[currentWave].descriptor;

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

void World::setOnBarrierDamageFunction(const std::function<void(float damage)>& func)
{
	m_onBarrierDamageReceived = func;
}

void World::setOnBarrierHealthGainedFunction(const std::function<void(float health)>& func)
{
	m_onBarrierHealthGained = func;
}

void World::setOnCurrencyUpdateFunction(const std::function<void(int currency)>& func)
{
	m_onCurrencyUpdate = func;
}

void World::setOnGameOverFunction(const std::function<void()>& func)
{
	m_onGameOver = func;
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
	m_waves.push_back(wave);
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

	m_onCurrencyUpdate(m_currency);
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
	const auto it = m_upgradeDefs.find(id);
	if (it == m_upgradeDefs.end())
		return;

	const float delta = it->second.delta;

	switch (id)
	{
		case UpgradeId::Laser_Damage:        m_laserUpgrades.damageUpgrade += delta; break;
		case UpgradeId::Laser_Range:         m_laserUpgrades.rangeUpgrade  += delta; break;

		case UpgradeId::Barrier_MaxHealth:   m_barrierUpgrades.maxHealthUpgrade += delta; break;
		case UpgradeId::Barrier_RegenAmount: m_barrierUpgrades.regenAmountUpgrade += delta; break;

		case UpgradeId::Cannon_Damage:       m_cannonUpgrades.damageUpgrade += delta; break;
		case UpgradeId::Cannon_FireRate:     m_cannonUpgrades.cooldownReduction += delta; break;

		default: break;
	}

	if (m_station)
	{
		for (auto& w : m_station->getWeapons())
		{
			if (auto* laser = dynamic_cast<Laser*>(w.get()))
				laser->setUpgrades(m_laserUpgrades);
			else if (auto* cannon = dynamic_cast<Cannon*>(w.get()))
				cannon->setUpgrades(m_cannonUpgrades);
			else if (auto* barrier = dynamic_cast<Barrier*>(w.get()))
				barrier->setUpgrades(m_barrierUpgrades);
		}
	}
}

int World::getUpgradeCost(const UpgradeId id) const
{
	const auto itDef = m_upgradeDefs.find(id);
	const auto itState  = m_upgradeState.find(id);

	/* Undefined, then invalid : 999999 */
	if (itDef == m_upgradeDefs.end() || itState == m_upgradeState.end())
		return 999999;

	const UpgradeDefinition& definition = itDef->second;
	const UpgradeState& state = itState->second;

	/* If upgrade is maxed out, invalid : 999999 cost */
	if (state.level >= definition.maxLevel)
		return 999999;

	auto cost = static_cast<float>(definition.baseCost);
	for (int i = 0; i < state.level; ++i)
		cost *= definition.costScale;

	return static_cast<int>(std::lround(cost));
}

bool World::tryBuyUpgrade(const UpgradeId id)
{
	const auto itDef = m_upgradeDefs.find(id);
	const auto itSt  = m_upgradeState.find(id);

	if (itDef == m_upgradeDefs.end() || itSt == m_upgradeState.end())
		return false;

	UpgradeState& state = itSt->second;
	const UpgradeDefinition& def = itDef->second;

	if (state.level >= def.maxLevel)
		return false;

	const int cost = getUpgradeCost(id);
	if (m_currency < cost)
		return false;

	m_currency -= cost;

	state.level++;

	applyUpgrade(id);

	if (m_onCurrencyUpdate)
		m_onCurrencyUpdate(m_currency);

	return true;
}

std::string World::getUpgradeName(const UpgradeId id) const
{
	const auto it = m_upgradeDefs.find(id);
	return (it != m_upgradeDefs.end()) ? it->second.uiName : "???";
}

int World::getUpgradeLevel(const UpgradeId id) const
{
	const auto it = m_upgradeState.find(id);
	return (it != m_upgradeState.end()) ? it->second.level : 0;
}

int World::getUpgradeMaxLevel(const UpgradeId id) const
{
	const auto it = m_upgradeDefs.find(id);
	return (it != m_upgradeDefs.end()) ? it->second.maxLevel : 0;
}
