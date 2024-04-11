#include "vector9.h"

#include <iostream>

int main()
{
    Wektor v = {1, 2, 4, 8, 16};
    for (int i = 0; i < v.size(); i++) std::cout << v[i] << " ";
    std::cout << "\n";

    Wektor a;
    Wektor b = 7;
    Wektor c(7);
    Wektor d = {7};
    Wektor e{7};
    auto f = Wektor(7);
    auto g = Wektor{7};

    Wektor h({7});
    Wektor j(2, 3);
    Wektor k{2, 3};
    Wektor l{1, 4, 6};
}
