// wersja ze strukturami, bardzo wstępna, poglądowa

#include <cstddef>  // <-- tu jest deklaracja size_t

struct Wektor
{
    int* dane;
    size_t size;
};

void init(Wektor& v, size_t size, int n0 = 0)
{
    v.dane = new int[size];
    v.size = size;
    for (size_t i = 0; i < size; i++)
    {
        v.dane[i] = n0;
    }
}

void free(Wektor& v)
{
    delete[] v.dane;
    v.dane = nullptr;
    v.size = 0;
}

void set_value(const Wektor& v, size_t index, int value) { v.dane[index] = value; }

int get_value(Wektor& v, size_t index) { return v.dane[index]; }

int main()
{
    Wektor w;
    init(w, 10);
    set_value(w, 5, 2);
    set_value(w, 3, get_value(w, 5) * 4);
    free(w);
}
