#include "vector7.h"

#include <stdexcept>

Wektor::Wektor() {}

Wektor::Wektor(size_t new_size, int init_value)
    : _size(new_size)
{
    _dane = new int[_size];
    for (size_t i = 0; i < _size; i++)
    {
        _dane[i] = init_value;
    }
}

Wektor::Wektor(const Wektor& v)
    : _size(v._size)
{
    _dane = new int[_size];
    for (size_t i = 0; i < _size; i++)
    {
        _dane[i] = v._dane[i];
    }
}

Wektor::~Wektor()
{
    delete[] _dane;
    _dane = nullptr;
    _size = 0;
}

Wektor& Wektor::operator=(const Wektor& rhs)
{
    if (this == &rhs) return *this;

    if (size() != rhs.size())
    {
        delete[] _dane;
        _dane = new int[rhs.size()];
    }
    for (size_t i = 0; i < rhs.size(); i++)
    {
        _dane[i] = rhs[i];
    }
    _size = rhs.size();
    return *this;
}

int& Wektor::operator[](size_t index) { return _dane[index]; }

int Wektor::operator[](size_t index) const { return _dane[index]; }

int& Wektor::at(size_t index)
{
    if (index >= _size) throw std::out_of_range("Index out of range in Wektor::at(size_t)");
    return _dane[index];
}

int Wektor::at(size_t index) const
{
    if (index >= _size) throw std::out_of_range("Index out of range in Wektor::at(size_t)");
    return _dane[index];
}

int& Wektor::front() { return _dane[0]; }

int Wektor::front() const { return _dane[0]; }

int& Wektor::back() { return _dane[_size - 1]; }

int Wektor::back() const { return _dane[_size - 1]; }

size_t Wektor::size() const { return _size; }

int* Wektor::data() const { return _dane; }

bool Wektor::empty() const { return _size == 0; }
