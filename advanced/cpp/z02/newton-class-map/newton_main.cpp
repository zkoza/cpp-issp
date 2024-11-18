#include "newton.h"

#include <format>
#include <iostream>

int main()
{
    Newton newton;
    int last = 16;
    for (int k = 1; k <= last; k++)
    {
        auto n = 2 * k;
        auto result = newton(n, k);
        auto msg = std::format("newton({}, {}) = {}\n", n, k, result);
        std::cout << msg;
    }
    std::cout << "memoized: " << newton.memoized_elements() << "\n";
}
