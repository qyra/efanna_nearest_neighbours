#pragma once

#include <vector>
#include <deque>

#include "efanna_config.hpp"
#include "types.hpp"
#include "heap.hpp"

//Could make this a union for better perf.
//Either needs high, low, pivot_id
//Or ids
//Never both.
struct KDNode
{
    KDNode* high = nullptr;
    KDNode* low = nullptr;
    IDList* ids = nullptr;
    int pivot_id = -1;
};

class KDTree
{
public:
    KDTree(PointList points);
    ~KDTree();
    void query(std::deque<std::deque<int>>& ids, std::deque<std::deque<float>>& costs, const PointList& targets, int k);

private:
    void rQuery(BoundedHeap& results, KDNode* node, const Point& target, int depth);
    float distance(const Point& a, const Point& b);

    void delete_node(KDNode* x);
    void print_id_list(IDList ids, int dim, std::string prefix);

    void median_split(IDList& high_ids, IDList& low_ids, int& pivot_id, IDList& ids, int dim);
    int partition(IDList& ids, int left_i, int right_i, int dim);

    int measure_dimensions();
    KDNode* create_tree(IDList* ids, int depth);

    KDNode* root;
    PointList points;
    int dimensions;
    int leaf_count = 3;
};
