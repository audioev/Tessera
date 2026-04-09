//
// Created by Evan Neumann on 10/03/2026.
//

#pragma once
#include "Grain.h"

class GrainPool
{
    public:
    GrainPool();
    ~GrainPool();

    void prepare();
    Grain* getInactiveGrain();
    void returnGrain(Grain* grain);
    auto begin(){return grains.begin();}
    auto end(){return grains.end();}

private:
    std::array<Grain, 256> grains;
};


