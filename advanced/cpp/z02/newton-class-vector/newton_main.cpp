#include "newton.h"

#include <format>
#include <iostream>

int main()
{
    Newton newton;
    for (int k = 1; k < 17; k++)
    {
        auto n = 2 * k;
        auto result = newton(n, k);
        auto msg = std::format("newton({}, {}) = {}\n", n, k, result);
        std::cout << msg;
    }
}
