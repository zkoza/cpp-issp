#include <iostream>

// konstruktory, destruktor

class Wektor
{
    int* dane;
    size_t size;

  public:
    Wektor()   // konstruktor bezargumentowy
    {
        dane = nullptr;
        size = 0;
    }

    Wektor(size_t new_size, int init_value = 0) // popularny konstruktor z argumentem domyślnym
    {
        size = new_size;
        dane = new int[new_size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = init_value;
        }
    }

    Wektor(const Wektor& v) // konstruktor kopiujący
    {
        size = v.size;
        dane = new int[size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = v.dane[i];
        }
    }

    ~Wektor()  // destruktor
    {
        delete[] dane;
        dane = nullptr;
        size = 0;
    }
};

int main()
{
    // test, czy kontruktory sie kompilują
    Wektor w(5);
    Wektor v(w);
    Wektor u;
}
