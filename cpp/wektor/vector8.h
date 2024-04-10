#ifndef VECTOR8_H
#define VECTOR8_H


#include <cstddef>
#include <stdexcept>

class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

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
    size_t capacity() const;
    void resize(size_t new_size, int init_value = 0);
    void shrink_to_fit();
    void push_back(int value);
    void pop_back();
  private:
    void grow();
};

#endif  // VECTOR8_H
