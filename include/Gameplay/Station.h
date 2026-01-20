//
// Created by Oier Álvarez on 19/01/2026.
//

#ifndef SPACESTATIONDEFENSE_STATION_H
#define SPACESTATIONDEFENSE_STATION_H
#include "Entity.h"


class Station : public Entity
{
public:
    struct StationDescriptor : EntityDescriptor
    {
        float radius = 0.f;
    };

    bool init(const StationDescriptor& descriptor);
    void update(float dt) override;
};


#endif //SPACESTATIONDEFENSE_STATION_H