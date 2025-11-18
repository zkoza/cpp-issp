#include <algorithm>
#include <list>

int main()
{
    std::list v = {1, 5, 2, 6};
    std::ranges::sort(begin(v), end(v));
}
