#ifndef SPACESTATIONDEFENSE_WORLD_H
#define SPACESTATIONDEFENSE_WORLD_H

#include <cstdint>
#include <functional>
#include <list>

#include "ObjectPool.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/Entity.h"
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
	World() = default;
	~World() = default;

	// Minimal load: nothing to load yet
	bool load();

	// deltaMilliseconds kept to match your template
	void update(float deltaMilliseconds);

	// Minimal render: draw background only (optional)
	void render(sf::RenderWindow& window);

	void setStation(std::unique_ptr<Station> station);
	void spawnEnemy(const Enemy::EnemyDescriptor& baseDesc, sf::Vector2f targetPos, float targetSize);

	[[nodiscard]] const Station* getStation() const;

	void setOnDamageFunction(const std::function<void(float damage)>& func);

	void setAimWorld(const sf::Vector2f& aimWorld);
	void onLeftClick();

protected:
	std::function<void(float damage)> m_onHealthDamageReceived;
	ObjectPool<Enemy> m_enemyPool {128};
	std::unique_ptr<Station> m_station;

};

#endif // SPACESTATIONDEFENSE_WORLD_H
