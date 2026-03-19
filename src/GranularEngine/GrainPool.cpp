//
// Created by Evan Neumann on 10/03/2026.
//

#include "GrainPool.h"

GrainPool::GrainPool(){}

GrainPool::~GrainPool(){}

void GrainPool::prepare()
{
    for (Grain grain : grains)
    {
        grain.setActive(false);
    }
}

Grain* GrainPool::getInactiveGrain()
{
    for (auto grain : grains)
    {
        if (grain.getActive() == false)
        {
            return &grain;
        }
    }
    return nullptr;
}

void GrainPool::returnGrain(Grain* grain)
{
    grain->setActive(false);
}
