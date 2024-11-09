#include <iostream>

struct Element
{
    int value;
    Element* next;
};

void insert(Element*& root, int n)
{
    Element* tmp = new Element{.value = n, .next = root};
    root = tmp;
}

void free(Element*& root)
{
    if (root == nullptr)
        return;
    free(root->next);
    delete root;
    root = nullptr;
}

void free_in_loop_goto(Element*& root)
{
start:
    if (root != nullptr)
    {
        auto tmp = root;
        root = root->next;
        delete tmp;
        goto start;
    }
}

void free_in_loop(Element*& root)
{
    while (root != nullptr)
    {
        auto tmp = root;
        root = root->next;
        delete tmp;
    }
}

int suma_loop(const Element* root)
{
    int wynik = 0;
    while (root != nullptr)
    {
        wynik += root->value;
        root = root->next;
    }
    return wynik;
}

int suma(const Element* root)
{
    if (root == 0)
        return 0;
    return root->value + suma(root->next);
}

void zero(Element* root)
{
    if (root == 0)
        return;
    root->value = 0;
    zero(root->next);
}

void zero_loop(Element* root)
{
    while (root != nullptr)
    {
        root->value = 0;
        root = root->next;
    }
}


int main()
{
    Element* root = nullptr;
    insert(root, 37);
    insert(root, 99);
    insert(root, 12);

    std::cout << suma(root) << "\n";
    zero_loop(root);
    std::cout << suma(root) << "\n";


    free_in_loop(root);

    std::cout << "koniec\n";
}


// int main()
// {
//     Element e{.value = 37, .next = nullptr};
//     Element* root = &e;

//     insert(root, 99);
//     insert(root, 12);
//     std::cout << suma(root) << "\n";

//     free_in_loop(root);
//     std::cout << "koniec\n";
// }
