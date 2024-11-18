#include "newton.h"

int Newton::operator()(int n, int k)
{
    if (k == 0 || k == n)
        return 1;

    if (n >= tab.size())
    {
        auto old_size = tab.size();
        tab.resize(n + 1);
        for (int i = old_size; i < tab.size(); i++)
        {
            tab[i].resize(i + 1, invalid_value());
        }
    }

    if (tab[n][k] < 0)
    {
        tab[n][k] = operator()(n - 1, k - 1) + operator()(n - 1, k);
    }
    return tab[n][k];
}
