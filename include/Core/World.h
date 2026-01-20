#ifndef SPACESTATIONDEFENSE_WORLD_H
#define SPACESTATIONDEFENSE_WORLD_H

#include <cstdint>
#include <list>

#include "Gameplay/Entity.h"
#include "Gameplay/Station.h"

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

	void addEntity(std::unique_ptr<Entity> entity);
	void setStation(std::unique_ptr<Station> station);
	[[nodiscard]] const Station* getStation() const;

protected:
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unique_ptr<Station> m_station;

};

#endif // SPACESTATIONDEFENSE_WORLD_H
