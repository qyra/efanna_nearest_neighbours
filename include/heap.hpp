#pragma once

#include "neighbour.hpp"

#include <vector>
#include <cfloat>
#include <deque>
#include <set>
#include <ostream>

class BoundedHeap
{
public:
    BoundedHeap(IDType max_items);
    BoundedHeap();
    void resize(IDType max_items);

    void insert(IDType id, float cost);
    void collect_neighbours(std::deque<float>& costs, std::deque<int>& ids);
    float tau();

private:
    void sift_up(int i);
    void sift_down();
    void swap(int a, int b);
    void validate_heap();

public:
    std::set<IDType> ids;
    int max_items;
    int curr_items = 0;
    float max_cost = FLT_MAX;

    std::vector<Neighbour> data;
};

std::ostream& operator<<(std::ostream& o, const BoundedHeap& b);
