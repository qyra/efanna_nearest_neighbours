#pragma once

#include <vector>
#include <cfloat>

struct HeapItem
{
    int value;
    float cost;
};

class Heap
{
public:
    Heap(int max_items);
    ~Heap();

    void insert(int value, float cost);

    void printHeap();

private:
    void siftUp(int i);
    void siftDownTop();
    void swap(int a, int b);

public:
    int max_items;
    int curr_items = 0;
    float max_cost = FLT_MAX;

    std::vector<HeapItem> data;
};
