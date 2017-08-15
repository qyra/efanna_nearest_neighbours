#include "neighbour.hpp"

Neighbour::Neighbour(IDType id_in, float cost_in):
id(id_in),
cost(cost_in)
{
}

Neighbour::Neighbour():
id(0), cost(0.f)
{
}
