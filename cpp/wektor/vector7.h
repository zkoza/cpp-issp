#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;

  public:
    Wektor();
    Wektor(size_t new_size, int init_value = 0);
    Wektor(const Wektor& v);
    ~Wektor();

    Wektor& operator=(const Wektor& rhs);
    int& operator[](size_t index);
    int operator[](size_t index) const;
    int& at(size_t index);
    int at(size_t index) const;
    int& front();
    int front() const;
    int& back();
    int back() const;
    size_t size() const;
    int* data() const;
    bool empty() const;
};

#endif  // VECTOR7_H
