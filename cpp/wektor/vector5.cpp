// proste składowe const i nie-const

#include <iostream>
#include <stdexcept>

class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;

  public:
    Wektor() {}

    Wektor(size_t new_size, int init_value = 0)
        : _size(new_size)  // <--- lista inicjalizacyjna (preambuła) konstruktora
    {
        _dane = new int[_size];
        for (size_t i = 0; i < _size; i++)
        {
            _dane[i] = init_value;
        }
    }

    Wektor(const Wektor& v)
        : _size(v._size)  // <--- lista inicjalizacyjna (preambuła) konstruktora
    {
        _dane = new int[_size];
        for (size_t i = 0; i < _size; i++)
        {
            _dane[i] = v._dane[i];
        }
    }

    ~Wektor()
    {
        delete[] _dane;
        _dane = nullptr;
        _size = 0;
    }

    int& operator[](size_t index) { return _dane[index]; }
    int operator[](size_t index) const { return _dane[index]; }
    int& at(size_t index)
    {
        if (index >= _size) throw std::out_of_range("Index out of range in Wektor::at(size_t)");
        return _dane[index];
    }
    int at(size_t index) const
    {
        if (index >= _size) throw std::out_of_range("Index out of range in Wektor::at(size_t)");
        return _dane[index];
    }
    int& front() { return _dane[0]; }           // referencja do pierwszego elementu wektora
    int front() const { return _dane[0]; }      // wartość pierwszego elementu wektora
    int& back() { return _dane[_size-1]; }      // referencja do ostatniego elementu wektora
    int back() const { return _dane[_size-1]; } // wartość ostatniego elementu wektora
    size_t size() const { return _size; }       // rozmiar wektora
    int* data () const {return _dane; }         // wskaźnik do lokalizacji danych
    bool empty() const { return _size == 0; }   // czy wektor jest pusty?
};

void test(const Wektor& w)
{
    std::cout << w[0] << "\n";
    std::cout << w.size() << "\n";
    std::cout << w.empty() << "\n";
}

int main()
{
    Wektor w(5);
    test(w);
    w[1] = 8;
    w.front() = 1;
    w.back() = -1;
}
