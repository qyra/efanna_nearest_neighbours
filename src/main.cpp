// A simple program that computes the square root of a number

#include <iostream>
#include <libconfig.h++>
#include <random>

#include "efannaConfig.h"
#include "kdtree.hpp"
#include "heap.hpp"

void loadConfig(struct configStruct& options){
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

int main (int argc, char *argv[])
{
    struct configStruct options;
    loadConfig(options);
    std::cout << "pi == " << options.pi << std::endl;

    KDTree kdtree(options);
    std::cout << "Root: " << kdtree.root << std::endl;

    Heap heap(5);
    std::cout << "Max items: " << heap.max_items << std::endl;

    std::mt19937 gen(0); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 100);

    for(int i = 0; i < 10; i = i + 1){
        float cost = dis(gen);
        int val = dis(gen);
        std::cout <<"Inserting " << cost << ", " << val << std::endl;
        heap.insert(val, cost);
        heap.printHeap();
        std::cout << std::endl << std::endl;
    }

    return 0;
}
