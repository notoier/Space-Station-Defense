//
// Created by Oier Álvarez on 19/01/2026.
//

#include "Gameplay/Station.h"

bool Station::init(const StationDescriptor& descriptor)
{
    m_size = descriptor.radius;
    return Entity::init(descriptor);
}

void Station::update(float dt)
{
    Entity::update(dt);
}
