#include "klejnoty.h"

#include <iostream>

int main()
{
    std::vector c = {4, 2, 300, 1, 2, 2, 300};
    Klejnoty klejnoty(c);
    std::cout << klejnoty.optimal_price() << "\n";
}
