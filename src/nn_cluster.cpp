#include <iostream>
#include <algorithm>

#include "nn_cluster.hpp"
#include "avx_distance.hpp"


const void print_clusters(const std::vector<BoundedHeap>& clusters)
{

    int id = 0;
    double all_cost = 0;
    for(auto const& c: clusters){
        double line_cost = c.total_cost();
        all_cost += line_cost;
        std::cout << "Source: " << id << " Line Cost: " << line_cost << " Neighbours: " << c << std::endl;
        ++id;
    }
    std::cout << "All Cost = " << all_cost << std::endl << std::endl;
}

void print_set(std::set<IDType> s)
{
    for(auto const& x: s){
        std::cout << x << ", ";
    }
    std::cout << std::endl;
}

void NNCluster::expand_results(BoundedHeap& results, const Point& target)
{
    std::set<IDType> visited;
    visited.insert(results.ids.begin(), results.ids.end());
    std::set<IDType> frontier(visited);

    for(int i = 0; i < num_result_expansions; ++i){
        std::set<IDType> frontier_candidates;
        //Enumerate IDs of all neighbours of visited nodes.
        for(auto const& id: frontier){
            for(auto const& cousin_id: clusters[id].ids){
                frontier_candidates.insert(cousin_id);
            }
        }

        visited.insert(frontier.begin(), frontier.end());
        frontier.clear();

        if(i < num_result_expansions - 1){
            //Remove any nodes that were in frontier from new_frontier
            std::set_difference(frontier_candidates.begin(), frontier_candidates.end(),
                visited.begin(), visited.end(),
                std::inserter(frontier, frontier.end()));
        }
    }

    for(auto const& candidate: visited){
        float dist = distance(target, points[candidate], dimensions);
        results.insert(candidate, dist);
    }
}

void NNCluster::query(std::deque<std::deque<IDType>>& ids, std::deque<std::deque<float>>& costs, const PointList& targets, int k)
{
    ids.resize(targets.size());
    costs.resize(targets.size());

    std::mt19937 gen;
    gen.seed(0);
    std::uniform_int_distribution<IDType> rand_index(0, n-1);

    for(int target_i = 0; target_i < targets.size(); ++target_i){
        const Point& target = targets[target_i];

        BoundedHeap results;
        results.resize(k);
        for(int i = 0; i < candidate_init_count; ++i){
            InsertResult res = InsertResult::DUPLICATE;
            while(res != InsertResult::SUCCESS){
                IDType candidate = rand_index(gen);
                float dist = distance(target, points[candidate], dimensions);
                res = results.insert(candidate, dist);
            }
        }

        expand_results(results, target);
        results.collect_neighbours(costs[target_i], ids[target_i]);
    }
}

NNCluster::NNCluster(PointList points_in):
points(points_in)
{
    dimensions = measure_dimensions();
    n = points.size();
    std::mt19937 gen;
    gen.seed(0);
    std::uniform_int_distribution<int> rand_index(0, n-1);

    clusters.resize(n);

    if(num_close >= n-1){
        std::cout << "num_close = " << num_close << " and num points=" << n-1 << std::endl;
        throw std::invalid_argument("Num close must be < num points");
    }

    for(int id = 0; id < n; ++id){
        //Initialize neighbour heap
        clusters[id].resize(num_close);
        //Construct initial candidates per ID
        for(int near_i = 0; near_i < num_close; ++near_i){
            //Generate candidates until insert succeeds. (Not a duplicate)
            //This will infinitely loop if num_close < n-1
            InsertResult res = InsertResult::DUPLICATE;
            while(res != InsertResult::SUCCESS){
                IDType candidate = rand_index(gen);
                if(candidate != id){
                    float dist = distance(points[id], points[candidate], dimensions);
                    res = clusters[id].insert(candidate, dist);
                }
            }
        }
    }

    for(int i = 0; i < num_tree_expansions; ++i){
        nn_expand();
    }
}

void NNCluster::nn_expand()
{
    for(int id = 0; id < n; ++id){
        //Enumerate IDs of all neighbours of neighbours.
        std::set<IDType> candidates;

        for(auto const& neighbour_id: clusters[id].ids){
            for(auto const& candidate: clusters[neighbour_id].ids){
                candidates.insert(candidate);
            }
        }

        for(auto const& candidate: candidates){
            if(candidate != id){
                float dist = distance(points[id], points[candidate], dimensions);
                clusters[id].insert(candidate, dist);
            }
        }
    }
}

int NNCluster::measure_dimensions(){
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
