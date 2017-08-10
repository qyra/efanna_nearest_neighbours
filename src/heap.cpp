#include "heap.hpp"

#include <iostream>
#include <assert.h>

#include "efanna_config.hpp"

BoundedHeap::BoundedHeap (int max_items):
max_items(max_items)
{
    data.resize(max_items);
}

void BoundedHeap::print_heap()
{
    for(int i = 0; i < curr_items; ++i){
        std::cout <<"i=" << i << " cost=" << data[i].cost << " val=" << data[i].value << std::endl;
    }
}

float BoundedHeap::tau()
{
    //Cost of the furthest neighbour currently under consideration as a candidate.
    return max_cost;
}

void BoundedHeap::validate_heap(){
    //Check Invariant of Heaps
    for(int i = 0; i < curr_items; ++i){
        if(i > 0){
            int parent = (i-1) / 2;
            if(data[parent].cost < data[i].cost){
                std::cout <<"Error in heap, printing" << std::endl;
                print_heap();
                throw std::invalid_argument("Heap Invalid.");
            }
        }
    }
}

void BoundedHeap::collect_neighbours(std::deque<float>& costs, std::deque<int>& ids)
{
    costs.clear();
    ids.clear();
    while(curr_items > 0){
        HeapItem furthest = data[0];
        costs.push_front(furthest.cost);
        ids.push_front(furthest.value);

        data[0] = data[curr_items-1];
        --curr_items;
        sift_down();
    }
}

void BoundedHeap::insert(int value, float cost)
{
    if (curr_items >= max_items){
        //Always discard the worst item once the heap is full.
        if (cost > max_cost){
            return;
        }

        data[0].cost = cost;
        data[0].value = value;
        sift_down();
        max_cost = data[0].cost;
    } else {
        data[curr_items].cost = cost;
        data[curr_items].value = value;
        sift_up(curr_items);
        ++curr_items;

        if(curr_items > max_cost)
            max_cost = data[0].cost;
    }
}

void BoundedHeap::swap(int a, int b)
{
    HeapItem temp = data[a];
    data[a] = data[b];
    data[b] = temp;
}

void BoundedHeap::sift_up(int i)
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

void BoundedHeap::sift_down()
{
    int i = 0;
    while (true){
        int left_i = 2*i + 1;
        int right_i = 2*i + 2;
        //If left_i out of range, then this shouldn't consider swapping to it.
        //right_i > left_i
        if (right_i < curr_items){
            //2 children. Swap with the greater of them or stay put.
            if (data[i].cost > data[left_i].cost && data[i].cost > data[right_i].cost){
                break;
            } else if (data[left_i].cost > data[right_i].cost){
                //Swap with left child
                swap(left_i, i);
                i = left_i;
            } else {
                //Swap with right child.
                swap(right_i, i);
                i = right_i;
            }
        } else if (left_i < curr_items){
            if (data[i].cost < data[left_i].cost){
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
