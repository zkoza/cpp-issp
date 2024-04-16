#include "vector10.h"

#include <iostream>

int main()
{
    Wektor v = {1, 2, 4, 8, 16};
    for (auto& n : v)
    {
        n = 0;
    }
    for (auto n : v)
    {
        std::cout << n << " ";
    }

    std::cout << "\n";
}
