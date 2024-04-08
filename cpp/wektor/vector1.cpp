// wersja z klasą,
// - sekcje public/protected
// - funkce składowe

#include <iostream>

class Wektor
{
    int* dane;
    size_t size;

  public:
    auto get_size() { return size; }
    auto set_size(size_t new_size) { size = new_size; }
};

int main()
{
    Wektor w;
    w.set_size(6);
    std::cout << w.get_size() << "\n";
}
