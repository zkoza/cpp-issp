#include <algorithm>
#include <list>

int main()
{
    std::list v = {1, 5, 2, 6};
    std::sort(begin(v), end(v));
}
