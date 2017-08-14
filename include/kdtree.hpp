#pragma once

#include <vector>
#include <deque>

#include "efanna_config.hpp"
#include "types.hpp"
#include "heap.hpp"

//This ID is reserved for leaf nodes. Using 2^64  real nodes will
//cause the program to break.

struct KDNode
{
    IDType pivot_id;

    KDNode* high;
    //leaf_count will always be at least 2, therefore high and low can
    //only be used when there are no ids in this node. So it's possible
    //to use A union to reduce node size.
    //pivot_id will be used to differentiate child nodes from mid nodes,
    //pivot_id == LEAF_NODE_ID iff this is a leaf node.
    union {
        KDNode* low;
        IDList* ids;
    };
};

class KDTree
{
public:
    KDTree(PointList points);
    ~KDTree();
    void query(std::deque<std::deque<int>>& ids, std::deque<std::deque<float>>& costs, const PointList& targets, int k);

private:
    void r_query(BoundedHeap& results, KDNode* node, const Point& target, int depth);

    void delete_node(KDNode* x);
    void print_id_list(IDList ids, int dim, std::string prefix);

    void median_split(IDList& high_ids, IDList& low_ids, IDType& pivot_id, IDList& ids, int dim);
    int partition(IDList& ids, int left_i, int right_i, int dim);

    int measure_dimensions();
    KDNode* create_tree(IDList* ids, int depth);

    KDNode* root;
    PointList points;
    int dimensions;
    int leaf_count = 3;
};
