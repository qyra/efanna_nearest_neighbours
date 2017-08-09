#include "heap.hpp"

#include <iostream>

#include "efannaConfig.h"

Heap::Heap (int max_items):
max_items(max_items)
{
    data.resize(max_items);
}

Heap::~Heap ()
{

}

void Heap::printHeap()
{
    for(int i = 0; i < curr_items; ++i){
        std::cout <<"i=" << i << " cost=" << data[i].cost << " val=" << data[i].value << std::endl;
    }
}

void Heap::insert(int value, float cost)
{
    if (curr_items >= max_items){
        //Always discard the worst item once the heap is full.
        if (cost > max_cost)
            return;

        data[0].cost = cost;
        data[0].value = value;
        siftDownTop();

        max_cost = data[0].cost;
    } else {
        data[curr_items].cost = cost;
        data[curr_items].value = value;
        siftUp(curr_items);
        ++curr_items;

        if(curr_items > max_cost)
            max_cost = data[0].cost;
    }
}

void Heap::swap(int a, int b)
{
    HeapItem temp = data[a];
    data[a] = data[b];
    data[b] = temp;
}

void Heap::siftUp(int i)
{
    while (i > 0){
        int parent_i = (i+1)/2 - 1;

        if (data[i].cost > data[parent_i].cost){
            swap(parent_i, i);
            i = parent_i;
        } else {
            return;
        }
    }
}

void Heap::siftDownTop()
{
    int i = 0;
    while (true){
        int left_i = 2*i + 1;
        int right_i = 2*i + 2;
        //If left_i out of range, then this shouldn't consider swapping to it.
        //right_i > left_i
        if (right_i < max_items){

            if (data[left_i].cost > data[right_i].cost){
                if (data[left_i].cost > data[i].cost){
                    swap(left_i, i);
                    i = left_i;
                } else
                    break;

            } else {
                if (data[right_i].cost > data[i].cost){
                    swap(right_i, i);
                    i = right_i;
                } else
                    break;
            }
        } else if (left_i < max_items){
            if (data[left_i].cost > data[i].cost){
                swap(left_i, i);
                i = left_i;
            }
            //There was no left, so always break.
            break;

        } else {
            break;
        }
    }
}
