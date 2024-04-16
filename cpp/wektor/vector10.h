#ifndef VECTOR10_H
#define VECTOR10_H

#include <cstddef>
#include <initializer_list>

class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

  public:
    class const_iterator
    {
        int cur_index = 0;
        const Wektor& wektor;

      public:
        const_iterator(const Wektor& w, int pos = 0)
            : cur_index{pos},
              wektor{w}
        {
        }
        int operator*() const { return wektor[cur_index]; }
        void operator++() { cur_index++; }
        bool operator!=(const const_iterator& rhs) const { return cur_index != rhs.cur_index; }
    };

    class iterator
    {
        int cur_index = 0;
        Wektor& wektor;

      public:
        iterator(Wektor& w, int pos = 0)
            : cur_index{pos},
              wektor{w}
        {
        }
        int& operator*() { return wektor[cur_index]; }
        void operator++() { cur_index++; }
        bool operator!=(const iterator& rhs) const { return cur_index != rhs.cur_index; }
    };

    Wektor();
    Wektor(size_t new_size, int init_value = 0);
    Wektor(const Wektor& v);
    Wektor(const std::initializer_list<int>& list);
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
    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();

  private:
    void grow();
};

#endif  // VECTOR10_H
