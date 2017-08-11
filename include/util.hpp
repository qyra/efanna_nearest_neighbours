#pragma once

#include <sstream>

template<class T>
std::string string_vector(T& target)
{
    std::stringstream ss;
    ss << "(";
    for(int i = 0; i < target.size() -1; ++i){
        ss << target[i] << ", ";
    }
    if(target.size() > 0){
        ss << target[target.size() -1];
    }
    ss << ")";
    return ss.str();
}

template <class T>
void dumpobject(T const *t)
{
    unsigned char const *p = reinterpret_cast<unsigned char const *>(t);
    for (size_t n = 0 ; n < sizeof(T) ; ++n)
        printf("%02d ", p[n]);
    printf("\n");
}
