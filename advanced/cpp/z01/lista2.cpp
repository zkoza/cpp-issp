#include <iostream>

struct Element
{
    int value;
    Element* prev;
    Element* next;
};

struct List
{
    Element* head = nullptr;
    Element* tail = nullptr;
};

void insert(List& list, int n)
{
    Element* tmp = new Element{.value = n, .prev = nullptr, .next = list.head};
    const bool empty = list.head == nullptr;
    if (empty)
        list.tail = tmp;
    else
        list.head->prev = tmp;
    list.head = tmp;
}

void free(List& list)
{
    if (list.head == nullptr)
        return;
    auto tmp = list.head;
    list.head = list.head->next;
    delete tmp;
    free(list);
}

void print(const List& list)
{
    std::cout << "{ ";
    auto tmp = list.head;
    while (tmp != nullptr)
    {
        std::cout << tmp->value << " ";
        tmp = tmp->next;
    }
    std::cout << "}\n";
}

void print_backward(const List& list)
{
    std::cout << "{ ";
    auto tmp = list.tail;
    while (tmp != nullptr)
    {
        std::cout << tmp->value << " ";
        tmp = tmp->prev;
    }
    std::cout << "}\n";
}

void reverse(List& list)
{
    auto root = list.head;
    while (root != nullptr)
    {
        std::swap(root->next, root->prev);
        root = root->prev;  // po swapie root->prev jest elementem następnym!
    }
    std::swap(list.head, list.tail);
}

int main()
{
    List lista;
    insert(lista, 33);
    insert(lista, 22);
    insert(lista, 11);
    print(lista);
    print_backward(lista);
    reverse(lista);
    print(lista);
    print_backward(lista);

    free(lista);
}
