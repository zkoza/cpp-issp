#ifndef KLEJNOTY_H
#define KLEJNOTY_H

#include <vector>

class Klejnoty
{
  public:
    Klejnoty(std::vector<int> c)
        : c{c}
    {
        init_tab();
        _optimal_price = best_price(0, c.size() - 1);
    }
    int optimal_price() const { return _optimal_price; }

  private:
    std::vector<int> c;
    std::vector<std::vector<int>> tab;
    int _optimal_price = -1;

    int best_price(int first, int last);
    void init_tab();
};

#endif
