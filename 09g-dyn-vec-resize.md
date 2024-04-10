## Zmiana wielkości wektora

Prawdziwie użyteczna tablica dynamiczna to taka struktura danych, której wielkość można dowolnie zmieniać podczas wykonywania się programu, zależnie od bieżących potrzeb. Nie można tego jednak robić  efektywnie, mając do dyspozycji wyłącznie składowe `_dane` i `_size`.  Rozważmy bowiem typowy przykład: czytamy dane z pliku, nie wiedząc z góry, ile ich jest. Zapisujemy je w tablicy dynamicznej. Jeżeli mamy tylko  `_dane` i `_size`, to po wczytaniu kolejnej liczby (litery, etc.), musimy zwiększyć rozmiar tablicy o 1, czyli wywołać `operator[]`. Jest to jednak czasochłonna operacja, która może nawet wymagać "negocjacji" z systemem operacyjnym. 

Dużo lepsze rozwiązanie polega na zastosowaniu struktury danych z trzema składowymi:  `_dane`,  `_size` i `_capacity`. Pierwsza z nich tak jak poprzednio przechowuje adres pierwszego elementu tablicy zawierającej nasze dane. Składowa `_size` przechowuje tzw. rozmiar logiczny tablicy, czyli liczbę  elementów faktycznie w niej przechowywanych. Z kolei `_capacity` to tzw. rozmiar fizyczny tablicy dynamicznej, czyli liczbę elementów, jaką tablica jest w stanie pomieścić bez powiększania jej rozmiaru operatorem `new[]`. 

```c++
class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;
    // składowe funkcyjne
    ...
};
```

W czym to pomaga? Spójrzmy na typową implementację funkcji składowej `pop_back`, która usuwa z tablicy jej ostatni element:

```c++
void Wektor::pop_back()
{
    _size--;
}
```

Ta funkcja niczego fizycznie nie usuwa. Po prostu zmniejsza rozmiar logiczny tablicy, nie sprawdzając nawet, czy tablica nie jest pusta (C++ zakłada, że jeżeli na tablicy `tab` uruchomisz `pop_back()`, to albo masz pewność, że ona nie jest pusta, albo wcześniej upewniłeś się o tym, sprawdzając wartość wyrażenia  `tab.empty()`).  Funkcja `pop_back` jest więc niezwykle szybka, jednak cena, jaką za to płacimy, jest utrata informacji o rzeczywistym rozmiarze tablicy `tab`. Dlatego potrzebujemy dodatkowej składowej, `_capacity`, która przechowywać będzie właśnie tę wielkość. 

Spójrzmy teraz na możliwą implementację funkcji składowej `push_back(int)`, która dopisuje swój argument na końcu listy. Składa się ona z dwóch funkcji: publicznej `push_back(int)` oraz pomocniczej dla niej, prywatnej funkcji `grow()`:

```c++  
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

void Wektor::push_back(int value)
{
    if (size() == capacity())
    {
        grow();
    }
    _dane[_size] = value;
    _size++;
}
```

Pierwsza z nich podwaja fizyczny rozmiar tablicy dynamicznej, a w przypadku, gdy wynosi on zero, rozszerza ją do 1 elementu. Następnie alokuje pamięć na nowy bufor (tu: `tmp`, przepisuje do niego dane z aktualnego bufora (tu: `_dane`),  zamienia wartości tych dwóch buforów (`std::swap`) i usuwa stary bufor, którego adres, po swap-ie, znajduje się w zmiennej `tmp` (instrukcją `delete[] tmp;`). Podkreślmy: `grow` zasadniczo podwaja rozmiar tablicy. 

Mając `grow`, zaimplementowanie `push_back` jest już proste. Najpierw sprawdzamy, czy w tablicy jest miejsce na kolejny element. Jeśli nie, to podwajamy jej rozmiar (lub zwiększamy do 1, jeśli jest pusta)). Teraz na pewno w tablicy jest miejsce na kolejny element. Zapisujemy go na końcu tablicy i zwiększamy o 1 jej logiczny rozmiar. Koniec.

Składowa `_capacity` pozwala zaimplementować kilka pożytecznych składowych i w sumie wzbogacić klasę `Wektor` co najmniej o poniższe składowe:

```c++
  public:
    size_t capacity() const;
    void resize(size_t new_size, int init_value = 0);
    void shrink_to_fit();
    void push_back(int value);
    void pop_back();
  private:
    void grow();
```

- `capacity() const` zwraca po prostu wartość `_capacity`. 
- `resize(size_t new_size, int init_value = 0)`  zmienia rozmiar tablicy, przy czym jeżeli nowy rozmiar będzie większy od bieżącego, to nowe elementy tablicy wypełnia się elementami o wartości `init_value`.
- `shrink_to_fit` zmniejsza `_capacity` do wartości, jaką ma `_size`. 

Składowe te występują w klasie `std::vector` i są bardzo użyteczne, jednak ich implementacja nie wnosi niczego istotnego w zakresie języka C++, pomijam więc tu dalsze szczegóły.

Prosty program testujący:

```c++
#include "vector8.h"

#include <iostream>

int main()
{
    Wektor v;
    for (auto i : {1, 2, 3, 8})
        v.push_back(i);           // v = {1, 2, 3, 8}
    v.resize(10);                 // v = {1, 2, 3, 8, 0, 0, 0, 0, 0, 0}
    v.resize(6);                  // v = {1, 2, 3, 8, 0, 0}
    v.pop_back();                 // v = {1, 2, 3, 8, 0}
    for (int i = 0; i < v.size(); i++)
        std::cout << v[i] << "\n";
    std::cout << v.capacity() << "\n";   // 10
}
```

Linki do kodu źródłowego:

- [vector8.h](./cpp/wektor/vector8.h)

- [vector8.cpp](./cpp/wektor/vector8.cpp)

- [main8.cpp](./cpp/wektor/main8.cpp)

  

​       