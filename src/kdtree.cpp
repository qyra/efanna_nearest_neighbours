#include "kdtree.hpp"

#include <iostream>
#include <stdexcept>
#include <assert.h>
#include <math.h>
#include <cstdint> //for uintmax

#include "efanna_config.hpp"
#include "avx_distance.hpp"

const IDType LEAF_NODE_ID = UINT64_MAX;

int KDTree::measure_dimensions(){
    //Validate points.
    if (points.size() < 1){
        throw std::invalid_argument("No points");
    }

    int last_size = -1;

    for(Point p: points){
        if(last_size > 0){
            if(p.size() != last_size){
                throw std::invalid_argument("Differing point dimensions");
            }
        } else {
            last_size = p.size();

            if(last_size == 0){
                throw std::invalid_argument("Zero dimensional points prohibited");
            }
        }
    }
    return last_size;
}

KDTree::KDTree(PointList points):
points(points)
{
    dimensions = measure_dimensions();

    IDList* ids = new IDList;
    ids->resize(points.size());
    for(IDType i = 0; i < points.size(); ++i){
        (*ids)[i] = i;
    }

    root = create_tree(ids, 0);
}

KDTree::~KDTree()
{
    //Delete recursively.
    delete_node(root);
}

void KDTree::delete_node(KDNode* x){
    if(x == nullptr)
        return;

    if(x->pivot_id == LEAF_NODE_ID){
        if(x->ids != nullptr){
            delete(x->ids);
        }
    } else {
        if(x->low != nullptr)
            delete_node(x->low);

        if(x->high != nullptr)
            delete_node(x->high);

    }

    delete(x);
}

void KDTree::print_id_list(IDList ids, int dim, std::string prefix)
{
    for(auto const& i: ids){
        std::cout << prefix << "id=" << i << ", dimval=" << points[i][dim] << ", alldim=";
        for(int d = 0; d < dimensions; ++d){
            std::cout << points[i][d] << ", ";
        }
        std::cout << std::endl;

    }
}

void KDTree::r_query(BoundedHeap& results, KDNode* node, const Point& target, int depth)
{
    //Duplicate passed neighbours so left doesn't interfere with right.
    if (node->pivot_id == LEAF_NODE_ID){
        for(auto const& id: *(node->ids)){
            float dist = distance(target, points[id], dimensions);
            results.insert(id, dist);
        }
    } else {
        // Consider pivot as a neighbour
        // Check distance along only the one dimension.
        int dim = depth % dimensions;
        float dimension_distance = fabs(target[dim] - points[node->pivot_id][dim]);
        float euc_distance = distance(target, points[node->pivot_id], dimensions);

        results.insert(node->pivot_id, euc_distance);

        if (dimension_distance > results.tau()){
            // This is what makes kdtree search faster than brute force.
            // if the pivot is further than tau from target, and we are on the wrong side of the split, then we don't need to search that subtree.
            if (target[dim] <= points[node->pivot_id][dim])
                r_query(results, node->low, target, depth+1);

            if (target[dim] >= points[node->pivot_id][dim])
                r_query(results, node->high, target, depth+1);

        } else {
            // Search both subtrees, either could have a nearest neighbour.
            r_query(results, node->low, target, depth+1);
            r_query(results, node->high, target, depth+1);
        }
    }
}

void KDTree::query(std::deque<std::deque<int>>& ids,
                   std::deque<std::deque<float>>& costs,
                   const PointList& targets, int k)
{
    ids.resize(targets.size());
    costs.resize(targets.size());

    for(int i = 0; i < targets.size(); ++i){
        BoundedHeap results(k);
        r_query(results, root, targets[i], 0);
        results.collect_neighbours(costs[i], ids[i]);
    }
}

KDNode* KDTree::create_tree(IDList* ids, int depth){
    int n = ids->size();
    KDNode* node = new KDNode();

    if(n <= leaf_count){
        node->pivot_id = LEAF_NODE_ID;
        node->ids = ids;
        return node;
    } else {
        int dim = depth % dimensions;
        //Split across median value.
        IDList* high_ids = new IDList;
        IDList* low_ids = new IDList;
        median_split(*high_ids, *low_ids, node->pivot_id, *ids, dim);
        delete(ids);

        node->high = create_tree(high_ids, depth+1);
        node->low = create_tree(low_ids, depth+1);
        return node;
    }
}

void KDTree::median_split(IDList& high_ids, IDList& low_ids, IDType& pivot_id, IDList& ids, int dim)
{
    //Inputs: ids, dim
    //Outputs: left_ids, right_ids, pivot
    //Side effects: modifies order of ids
    //high_ids is populated with all ids such that points[id][dim] > points[pivot][dim}
    //low_ids is populated with all ids such that points[id][dim] < points[pivot][dim}
    //pivot is an id such that about half of points are below it, and half are above it on the dimension dim.
    //This uses a variant of quickselect to make it O(n)
    int n = ids.size();
    int m = n / 2;

    int left = 0;
    int right = n - 1;
    int j = 0;
    while (right > left){
        j = partition(ids, left, right, dim);

        if (j < m)
            left = j + 1;
        else if (j > m)
            right = j - 1;
        else
            break;
    }

    pivot_id = ids[m];
    for(int i = 0; i < m; ++i)
        low_ids.push_back(ids[i]);

    for(int i = m+1; i < n; ++i)
        high_ids.push_back(ids[i]);

}

int KDTree::partition(IDList& ids, int left_i, int right_i, int dim){
    int temp;
    int pivot_v = points[ ids[left_i] ][dim];
    int l_i = left_i;
    int r_i = right_i + 1;
    while (true){
        while (++l_i < right_i && points[ ids[l_i] ][dim] < pivot_v){}
        while(--r_i > left_i && points[ ids[r_i] ][dim] > pivot_v){}

        if (l_i >= r_i)
            break;

        temp = ids[l_i];
        ids[l_i] = ids[r_i];
        ids[r_i] = temp;
    }

    temp = ids[left_i];
    ids[left_i] = ids[r_i];
    ids[r_i] = temp;

    return r_i;
}
