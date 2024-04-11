#include "vector9.h"

#include <iostream>

int main()
{
    Wektor v = {1, 2, 4, 8, 16};
    for (int i = 0; i < v.size(); i++)
        std::cout << v[i] << "\n";
}
