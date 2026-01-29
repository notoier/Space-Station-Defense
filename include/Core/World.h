#ifndef SPACESTATIONDEFENSE_WORLD_H
#define SPACESTATIONDEFENSE_WORLD_H

#include <functional>
#include "ObjectPool.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/Entity.h"
#include "Gameplay/Projectile.h"
#include "Gameplay/Station.h"

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
	void spawnEnemy(sf::Vector2f targetPos, float targetSize);

	[[nodiscard]] const Station* getStation() const;

	void setOnDamageFunction(const std::function<void(float damage)>& func);

	void setAimWorld(const sf::Vector2f& aimWorld);
	void addWave(const Wave& wave);
	void onLeftClick();

protected:
	int m_currentWave = 0;
	int m_amountOfEnemies = 0;
	std::vector<Wave> waves;
	std::function<void(float damage)> m_onHealthDamageReceived;
	ObjectPool<Enemy> m_enemyPool {128};
	ObjectPool<Projectile> m_projectilePool {128};
	std::unique_ptr<Station> m_station;
};

#endif // SPACESTATIONDEFENSE_WORLD_H
