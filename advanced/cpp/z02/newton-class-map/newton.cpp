#include "newton.h"

int Newton::operator()(int n, int k)
{
    if (k == 0 || k == n)
        return 1;

    auto it = mapa.find({n, k});
    int result;
    if (it == mapa.end())
    {
        result = operator()(n - 1, k - 1) + operator()(n - 1, k);
        mapa.insert({{n, k}, result});
    }
    else
    {
        result = it->second;
    }
    return result;
}
