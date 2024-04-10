#include "vector7.h"

#include <iostream>

int main()
{
    Wektor v(4);
    Wektor w(6);

    w.front() = 7;
    w[1] = 1;
    w[2] = 2;
    v = w;
    std::cout << w[0] << " " << w[1] << "\n";
    std::cout << w.size() << "\n";
    std::cout << w.empty() << "\n";
    std::cout << w.Wektor::empty() << "\n";   // dziwactwo, nie pisz tak
}
