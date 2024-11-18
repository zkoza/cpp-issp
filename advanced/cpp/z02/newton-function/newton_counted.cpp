#include <iostream>
#include <format>

int cnt = 0;

int newton(int n, int k)
{
    cnt++;
    if (k < 0 || k > n)
        throw std::out_of_range(std::format("invalid function parameters: n = {}, k = {}", n, k));
    if (k == 0 || k == n)
        return 1;
    return newton(n - 1, k) + newton(n - 1, k - 1);
}

int main()
{
    std::cout << newton(5,3) << " " << ::cnt << "\n";
    ::cnt = 0;
    for (int i = 0; i < 17; i++)
    {
        std::cout << std::format("newton({}, {}) \t= {}, \t{}\n", 2 * i, i, newton(2 * i, i), ::cnt) << std::flush;
        ::cnt = 0;
    }
}
