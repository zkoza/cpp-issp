// operator indeksowania + składowa at

#include <iostream>
#include <stdexcept>

class Wektor
{
    int* dane = nullptr;
    size_t size = 0;

  public:
    Wektor() {}

    Wektor(size_t new_size, int init_value = 0)
        : size(new_size)  // <--- lista inicjalizacyjna (preambuła) konstruktora
    {
        dane = new int[size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = init_value;
        }
    }

    Wektor(const Wektor& v)
        : size(v.size)  // <--- lista inicjalizacyjna (preambuła) konstruktora
    {
        dane = new int[size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = v.dane[i];
        }
    }

    ~Wektor()
    {
        delete[] dane;
        dane = nullptr;
        size = 0;
    }

    int& operator[](size_t index) { return dane[index]; }

    int& at(size_t index)
    {
        if (index >= size) throw std::out_of_range("Index out of range in Wektor::at(size_t)");
        return dane[index];
    }
};

int main()
{
    Wektor w(5);
    w[1] = 8;
    std::cout << w[1] << "\n";
    std::cout << w[5] << "\n";  // tu jest błąd, program zapewne go nie zauważy
    w.at(1) = 1;
    w.at(10) = -1;  // tu program zgłosi wyjątek
}
