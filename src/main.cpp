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

void test_kd_tree()
{
    std::mt19937 gen(0); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 1000);

    struct ConfigStruct options;
    load_config(options);

    int dimensions = 10;
    int numpoints = 100000;
    int numtargets = 1;
    int k = 3;

    //~ std::cout << "Printing Points:" << std::endl;
    PointList points;
    for(int i = 0; i< numpoints; ++i){
        std::vector<int> point;

        for(int j = 0; j < dimensions; ++j){
            point.push_back(dis(gen));
        }
        points.push_back(point);
    }

    KDTree kdtree(points);

    PointList targets;
    for(int i = 0; i< numtargets; ++i){
        std::vector<int> target;

        for(int j = 0; j < dimensions; ++j){
            target.push_back(dis(gen));
        }
        targets.push_back(target);
    }

    std::deque<std::deque<int>> ids;
    std::deque<std::deque<float>> costs;

    kdtree.query(ids, costs, targets, k);

    std::cout <<"Results: " << std::endl;
    for(int i = 0; i < numtargets; ++i){
        std::cout << "Target:" << std::endl;
        std::cout << string_vector(targets[i]) << std::endl;

        for(int j = 0; j < k; ++j){
            std::cout << "Neighbour Point ID:" << ids[i][j] << std::endl;
            //~ std::cout << string_vector(points[ids[i][j]]) << std::endl;
            std::cout << "Cost:" << std::endl;
            std::cout << costs[i][j] << std::endl;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[])
{
    test_kd_tree();
    return 0;
}
