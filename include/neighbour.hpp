#pragma once

#include "types.hpp"

struct Neighbour
{
    Neighbour();
    Neighbour(IDType id, float cost);
    IDType id;
    float cost;
};

