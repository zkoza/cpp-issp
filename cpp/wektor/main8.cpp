#include "vector8.h"

#include <iostream>

int main()
{
    Wektor v;
    for (auto i : {1, 2, 3, 8})
        v.push_back(i);
    v.resize(10);
    v.resize(6);
    v.pop_back();
    for (int i = 0; i < v.size(); i++)
        std::cout << v[i] << "\n";
    std::cout << v.capacity() << "\n";
}
