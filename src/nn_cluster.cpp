#include <iostream>

#include "nn_cluster.hpp"
#include "avx_distance.hpp"

NNCluster::NNCluster(PointList points):
points(points)
{
    dimensions = measure_dimensions();
    IDType n = points.size();
    //~ gen.seed(rd());
    gen.seed(0);
    std::uniform_int_distribution<int> rand_index(0, n-1);

    clusters.resize(n);

    for(int id = 0; id < n; ++id){
        //Initialize neighbour heap
        clusters[id].resize(num_close);

        //Construct initial candidates per ID
        for(int near_i = 0; near_i < num_close; ++near_i){
            IDType candidate = rand_index(gen);
            float dist = distance(points[id], points[candidate], dimensions);
            clusters[id].insert(candidate, dist);
        }

        std::cout << clusters[id] << std::endl;
    }

    nn_expand();
}

void NNCluster::nn_expand()
{
    for(int id = 0; id < n; ++id){
        //Iterate through neighbours, add all neighbours of them.
        for(auto const& n_id: clusters[id].ids){
            for(auto const& candidate: clusters[n_id].ids){
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
