#include "neighbour.hpp"

Neighbour::Neighbour(IDType id, float cost):
id(id),
cost(cost)
{
}

Neighbour::Neighbour():
id(0), cost(0.f)
{
}
