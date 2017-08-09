// A simple program that computes the square root of a number
#include "efannaConfig.h"
#include <iostream>
#include <libconfig.h++>

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
    return 0;
}
