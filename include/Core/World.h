#ifndef SPACESTATIONDEFENSE_WORLD_H
#define SPACESTATIONDEFENSE_WORLD_H

#include <functional>
#include "ObjectPool.h"
#include "Gameplay/Barrier.h"
#include "Gameplay/Cannon.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/Entity.h"
#include "Gameplay/Laser.h"
#include "Gameplay/Projectile.h"
#include "Gameplay/Station.h"
#include "Gameplay/Upgrades.h"

class Enemy;
class Station;

namespace sf
{
	class RenderWindow;
}

class World
{
public:

	struct Wave
	{
		int amountOfEnemies{};
		Enemy::EnemyDescriptor descriptor;
	};

	World() = default;
	~World() = default;

	// Minimal load: nothing to load yet
	bool load();

	// deltaMilliseconds kept to match your template
	void update(float deltaMilliseconds);

	// Minimal render: draw background only (optional)
	void render(sf::RenderWindow& window);

	void setStation(std::unique_ptr<Station> station);
	void spawnEnemy();

	[[nodiscard]] const Station* getStation() const;

	void setOnDamageFunction(const std::function<void(float damage)>& func);
	void setOnBarrierDamageFunction(const std::function<void(float damage)>& func);
	void setOnBarrierHealthGainedFunction(const std::function<void(float health)>& func);
	void setOnCurrencyUpdateFunction(const std::function<void(int currency)>& func);
	void setTarget(sf::Vector2f targetPos, float targetSize);

	void setAimWorld(const sf::Vector2f& aimWorld) const;
	void addWave(const Wave& wave);

	void addCurrency(int money);
	void subCurrency(int money);
	[[nodiscard]] int getCurrency() const;

	void onEnemyDeath();
	void onLeftClick();
	void applyUpgrade(UpgradeId id);
	[[nodiscard]] int getUpgradeCost(UpgradeId id) const;
	bool tryBuyUpgrade(UpgradeId id);

	std::string getUpgradeName(UpgradeId id) const;
	int getUpgradeLevel(UpgradeId id) const;
	int getUpgradeMaxLevel(UpgradeId id) const;
protected:

	bool loadWavesFromJson(const std::string& filePath);
	bool loadStationAndWeaponsFromJson(const std::string& path);
	bool loadUpgradesFromJson(const std::string& path);
	void bindWeaponCallbacks();

	int m_currentWave = 0;
	int m_amountOfEnemies = 0;
	int m_currency = 0;
	bool m_waveActive = false;

	sf::Vector2f m_station_pos;
	float m_station_size{};

	Barrier* m_barrier {nullptr};

	Laser::LaserUpgrades m_laserUpgrades;
	Cannon::CannonUpgrades m_cannonUpgrades;
	Barrier::BarrierUpgrades m_barrierUpgrades;

	std::vector<Wave> waves;

	std::function<void(float damage)> m_onBarrierDamageReceived;
	std::function<void(float health)> m_onBarrierHealthGained;
	std::function<void(float damage)> m_onHealthDamageReceived;
	std::function<void(int currency)> m_onEnemyDeath;
	std::function<void(int currency)> m_onCurrencyUpdate;

	std::unordered_map<UpgradeId, UpgradeDefinition> m_upgradeDefs;
	std::unordered_map<UpgradeId, UpgradeState>      m_upgradeState;

	ObjectPool<Enemy> m_enemyPool {128};
	ObjectPool<Projectile> m_projectilePool {128};
	std::unique_ptr<Station> m_station;
};

#endif // SPACESTATIONDEFENSE_WORLD_H
