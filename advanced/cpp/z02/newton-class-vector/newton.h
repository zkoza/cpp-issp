#ifndef NEWTON_H
#define NEWTON_H

#include <vector>

class Newton
{
  public:
    Newton() = default;
    int operator()(int n, int k);

  private:
    constexpr int invalid_value() const { return -1; }
    std::vector<std::vector<int>> tab;
};

#endif  // NEWTON_H
