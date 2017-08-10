#pragma once

#include <vector>
#include <cfloat>
#include <deque>

struct HeapItem
{
    int value;
    float cost;
};

class BoundedHeap
{
public:
    BoundedHeap(int max_items);
    void insert(int value, float cost);
    void collect_neighbours(std::deque<float>& costs, std::deque<int>& ids);
    float tau();
    void print_heap();

private:
    void sift_up(int i);
    void sift_down();
    void swap(int a, int b);
    void validate_heap();

public:
    int max_items;
    int curr_items = 0;
    float max_cost = FLT_MAX;

    std::vector<HeapItem> data;
};
