#include "klejnoty.h"

#include <cassert>
#include <limits>

void Klejnoty::init_tab()
{
    const int invalid_value = -1;
    tab.resize(c.size());
    for (int i = 0; i < tab.size(); i++)
    {
        tab[i].resize(c.size(), invalid_value);
    }
}

int Klejnoty::best_price(int first, int last)
{
    assert(first >= 0);
    assert(last >= 0);
    assert(first < tab.size());
    assert(last < tab[first].size());
    assert(last >= first);

    if (tab[first][last] >= 0)
        return tab[first][last];
    if (last - first <= 1)
    {
        tab[first][last] = 0;
        return tab[first][last];
    }

    int current_best = std::numeric_limits<int>::max();
    for (int i = first + 1; i <= last - 1; i++)
    {
        auto left = best_price(first, i);
        auto right = best_price(i, last);
        auto current_emerald = c[first] * c[i] * c[last];
        current_best = std::min(current_best, left + right + current_emerald);
    }
    tab[first][last] = current_best;
    return tab[first][last];
}
