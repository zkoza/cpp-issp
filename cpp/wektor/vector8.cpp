#include "vector8.h"

#include <stdexcept>

Wektor::Wektor() {}

Wektor::Wektor(size_t new_size, int init_value) : _size(new_size), _capacity(new_size)
{
    _dane = new int[_capacity];
    for (size_t i = 0; i < _size; i++)
    {
        _dane[i] = init_value;
    }
}

Wektor::Wektor(const Wektor& v) : _size(v._size), _capacity(v._size)
{
    _dane = new int[_capacity];
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
    _capacity = 0;
}

Wektor& Wektor::operator=(const Wektor& rhs)
{
    if (this == &rhs) return *this;

    if (size() != rhs.size())
    {
        delete[] _dane;
        _capacity = rhs.size();
        _dane = new int[_capacity];
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

size_t Wektor::capacity() const { return _capacity; }

void Wektor::resize(size_t new_size, int init_value)
{
    if (new_size > capacity())
    {
        int *tmp = new int [new_size];
        for (size_t i = 0; i < size(); i++)
        {
            tmp[i] = _dane[i];
        }
        std::swap(_dane, tmp);
        delete [] tmp;
        _capacity = new_size;
    }

    for (size_t i = size(); i < new_size; i++)
    {
        _dane[i] = init_value;
    }
    _size = new_size;
}

void Wektor::shrink_to_fit()
{
    if (_size == _capacity) return;

    int *tmp = new int [_size];
    for (size_t i = 0; i < _size; i++)
    {
        tmp[i] = _dane[i];
    }
    std::swap(_dane, tmp);
    delete [] tmp;
    _capacity =  _size;
}

void Wektor::push_back(int value)
{
    if (size() == capacity())
    {
        grow();
    }
    _dane[_size] = value;
    _size++;
}

void Wektor::pop_back()
{
    _size--;
}

void Wektor::grow()
{
    _capacity = (_capacity > 0) ? 2 * _capacity : 1;
    int *tmp = new int [_capacity];
    for (size_t i = 0; i < size(); i++)
    {
        tmp[i] = _dane[i];
    }
    std::swap(_dane, tmp);
    delete [] tmp;
}

size_t Wektor::size() const { return _size; }

int* Wektor::data() const { return _dane; }

bool Wektor::empty() const { return _size == 0; }
