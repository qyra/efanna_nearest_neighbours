#pragma once

#include <random>
#include <set>
#include <vector>

#include "types.hpp"
#include "heap.hpp"

class NNCluster {
public:
    NNCluster(PointList points);
    void query(std::deque<std::deque<IDType>>& ids, std::deque<std::deque<float>>& costs, const PointList& targets, int k);

private:
    void nn_expand();
    void expand_results(BoundedHeap& results, const Point& target);
    int measure_dimensions();

    PointList points;
    std::random_device rd;

    std::vector<BoundedHeap> clusters;
    IDType n;
    const int num_close = 8;
    int candidate_init_count = 13;
    //~ const int num_close = 5;
    int num_result_expansions = 3;
    int num_tree_expansions = 3;
    int dimensions;
};
