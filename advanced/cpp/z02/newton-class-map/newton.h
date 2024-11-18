#ifndef NEWTON_H
#define NEWTON_H

#include <map>

class Newton
{
  public:
    Newton() = default;
    int operator()(int n, int k);
    int memoized_elements() const { return mapa.size(); }
  private:
    std::map<std::pair<int, int>, int> mapa;
};

#endif  // NEWTON_H
