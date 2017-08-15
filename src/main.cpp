// A simple program that computes the square root of a number

#include <iostream>
#include <libconfig.h++>
#include <random>
#include <deque>
#include <vector>

#include "efanna_config.hpp"
#include "kdtree.hpp"
#include "heap.hpp"
#include "types.hpp"
#include "util.hpp"
#include "nn_cluster.hpp"

void load_config(struct ConfigStruct& options)
{
    const char* cfgPath = "efanna.cfg";
    libconfig::Config config;

    try {
        //Docs:
        //http://www.hyperrealm.com/libconfig/libconfig_manual.html#The-C_002b_002b-API
        config.readFile(cfgPath);
        //These return false and do not change options if no match found.
        config.lookupValue("application.misc.pi", options.pi);
    } catch(const libconfig::FileIOException& e){
        std::cout << "Error loading config file: " << e.what() << std::endl;
    }
}

struct test_data {
    int dimensions = 10;
    int numpoints = 100000;
    int numtargets = 1;
    int k = 3;
    int seed = 0;
    int max_real = 1000;
    PointList points;
    PointList targets;
};

void generate_test_points(test_data& t){
    std::mt19937 gen(t.seed); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, t.max_real);

    for(int i = 0; i< t.numpoints; ++i){
        Point point;

        for(int j = 0; j < t.dimensions; ++j){
            point.push_back(dis(gen));
        }
        t.points.push_back(point);
    }

    for(int i = 0; i< t.numtargets; ++i){
        Point target;

        for(int j = 0; j < t.dimensions; ++j){
            target.push_back(dis(gen));
        }
        t.targets.push_back(target);
    }

}

void test_kd_tree()
{
    test_data t;
    generate_test_points(t);

    KDTree kdtree(t.points);

    std::deque<std::deque<int>> ids;
    std::deque<std::deque<float>> costs;

    kdtree.query(ids, costs, t.targets, t.k);

    std::cout <<"Results: " << std::endl;
    for(int i = 0; i < t.numtargets; ++i){
        std::cout << "Target:" << std::endl;
        std::cout << string_vector(t.targets[i]) << std::endl;

        for(int j = 0; j < t.k; ++j){
            std::cout << "Point ID: " << ids[i][j] << ", Cost: " << costs[i][j] << std::endl;
        }
        std::cout << std::endl;
    }
}

void test_clusters()
{
    test_data t;
    t.numpoints = 10;
    t.dimensions = 5;
    generate_test_points(t);

    NNCluster nncluster(t.points);

    //~ std::deque<std::deque<int>> ids;
    //~ std::deque<std::deque<float>> costs;

    //~ kdtree.query(ids, costs, targets, k);

    //~ std::cout <<"Results: " << std::endl;
    //~ for(int i = 0; i < numtargets; ++i){
        //~ std::cout << "Target:" << std::endl;
        //~ std::cout << string_vector(targets[i]) << std::endl;

        //~ for(int j = 0; j < k; ++j){
            //~ std::cout << "Neighbour Point ID:" << ids[i][j] << std::endl;
            //~ std::cout << string_vector(points[ids[i][j]]) << std::endl;
            //~ std::cout << "Cost:" << std::endl;
            //~ std::cout << costs[i][j] << std::endl;
        //~ }
        //~ std::cout << std::endl;
    //~ }
}

int main(int argc, char *argv[])
{
    test_kd_tree();
    test_clusters();
    return 0;
}
