#include <iostream>
#include <format>

int newton(int n, int k)
{
    if (k < 0 || k > n)
        throw std::out_of_range(std::format("invalid function parameters: n = {}, k = {}", n, k));
    if (k == 0 || k == n)
        return 1;
    return newton(n - 1, k) + newton(n - 1, k - 1);
}

int main()
{
    std::cout << newton(5, 3) << " " << "\n";

    for (int i = 0; i < 17; i++)
    {
        auto n = newton(2 * i, i);
        std::string msg = std::format("newton({}, {})\t= {}\n", 2 * i, i, n);
        std::cout << msg << std::flush;
    }
}
