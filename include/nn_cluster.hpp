#pragma once

#include <random>
#include <set>
#include <vector>

#include "types.hpp"
#include "heap.hpp"

class NNCluster {
public:
    NNCluster(PointList points);

private:
    void nn_expand();
    int measure_dimensions();

    PointList points;
    std::random_device rd;
    std::mt19937 gen;

    std::vector<BoundedHeap> clusters;
    IDType n;
    const int num_close = 5;
    int dimensions;
};
