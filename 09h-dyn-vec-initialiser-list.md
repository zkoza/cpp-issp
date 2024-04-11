## Konstruktor z listą inicjalizacyjną

Jak dotąd nasza klasa `Wektor` nie ma jednej użytecznej cechy: możliwości inicjalizowania tablicy dynamicznej listą elementów ujetych w nawiasy klamorowe. Innymi słowy, chcielibyśmy móc pisać coś w tym rodzaju:

```c++  
Wektor w = {1, 2, 4, 8, 16}; 
```

i oczekujemy, że w ten sposób utworzymy wektor 5-elementowy z kolejnymi elementami jak na liście inicjalizacyjnej. 

Rozwiązanie tego problemu jest proste. Należy (oczywiście!) napisać odpowiedni konstruktor. Mniej oczywiste jest to, że ten konstruktor musi pobierać  argument o specjalnym typie zdefiniowanym w bibliotece standardowej C++. Typ ten nazywa się `initializer_list` i jest generowany z szablonu, stąd konieczność użycia w jego specyfikacji nawiasów ostrokątnych, `<...>`, w którym umieszczamy typ elementów listy. Deklarację potrzebnego konstruktora umieszczamy oczywiście wewnątrz klasy w jej pliku nagłówkowy, Wygląda ona następująco:

```c++
Wektor(const std::initializer_list<int> & list);
```

Gdybyśmy tworzyli dynamiczny wektor elementów typu `'double`, w nawiasach ostrokątnych umieścilibyśmy `double` itd. 

Implementacja tego konstruktora jest dość prosta:

```c++
Wektor::Wektor(const std::initializer_list<int> &list)
{
    _size = list.size();
    _capacity = list.size();
    _dane = new int[_capacity];
    int index = 0;
    for (auto n: list)
    {
        _dane[index++] = n;
    }
}
```

Korzystamy to z tego, że lista inicjalizacyjna posiada składową `size` zwracającą liczbę elementów listy. Używamy jej do inicjalizacji rozmiaru logicznego (`_size`) i fizycznego (`_capacity`) listy. 

Możemy teraz przetestować nasz nowy konstruktor:

```c++ 
#include "vector9.h"
#include <iostream>

int main()
{
    Wektor v = {1, 2, 4, 8, 16};
    for (int i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";
    std::cout << "\n";
}
```

który kompiluje się i daje oczekiwany wynik

```txt
1 2 4 8 16
```

### Na marginesie: trzy, a nawet cztery sposoby inicjalizacji obiektów 

W tej chwili do inicjalizacji obiektów możemy korzystać z dowolnej z 4 składni:

```c++       
Wektor a(7);    // a
Wektor b{7};    // b
Wektor c = {7}; // c
Wektor d = 7;   // d  por: std::string s = "ala"
```

- W pierwszym przypadku (z nawiasami okrągłymi) zostanie wywołany konstruktor jednoargumentowy

- W drugim przypadku (z klamrami) mamy dwie możliwości

  - jeżeli w klasie Wektor zdefiniowano konstruktor z listą inicjalizacyjną, to zostanie wywołany ten konstruktor
  - w przeciwnym wypadku zostanie wywołany konstruktor jednoargumentowy (tu: z argumentem 7)

- Trzeci przypadek równoważny jest drugiemu (znak `=` nie jest operatorem przypisania)

- Przypadek czwarty, choć wygląda dziwnie, równoważny jest przypadkowi pierwszemu. Ten rodzaj inicjalizacji jest często widywany przy konstrukcji napisów, np.:

  ```c++
  std::string s = "Ala ma kota";
  ```

  Tu `s` jest bowiem obiektem pewnej klasy użytkownika, który inicjalizowany jest wyrażeniem zupełnie innego typu (zapewne `const char[12];`). Ten rodzaj inicjalizacji występuje też dość często przy przekazywaniu argumentów do funkcji przez wartość.  

Z powyższego wynika, że najczęściej nie ma żadnej różnicy, czy użyje się inicjalizatora w nawiasach okrągłych czy w klamrach, jednak jeżeli w klasie istnieje zarówno konstruktor z listą inicjalizacyjną, jak i z małą liczbą argumentów, to możemy mieć kłopot. Na przykład

```c++
Wektor w1(2);    // wektor 2-elementowy: {0, 0}
Wektor w2{2};    // wektor 1-elementowy: {2}
Wektor w3(1, 3); // wektor 1-elementowy: {3}
Wektor w4{1, 3}; // wektor 2-elementowy: {1, 3}
```

Ponadto, jeżeli pominiemy inicjalizator, np.:

```c++ 
Wektor w;
```

to obiekt zostanie skonstruowany tzw. konstruktorem domyślnym, czyli takim, który jest bezargumentowy lub którego wszystkie argumenty mają wartości domyślne. 

Żeby było jeszcze dziwniej, instrukcja

```c++
Wektor x();
```

nie definiuje obiektu, lecz jest deklaracją funkcji o nazwie `x` i wartości typu `Wektor`. 

Widuje się też definicje obiektów z typem dedukowanym przez kompilator:

```c++
auto f = Wektor(7);  // konstruktor 1-argumentowy => {0, 0, 0, 0, 0, 0, 0}
auto g = Wektor{7};  // konstruktor z listą inicjalizacyjną => {7}
```

Wszytko to razem sprawia pewne wrażenie chaosu. Jego źródłem są decyzje podejmowane w kolejnych wersjach C++. Pierwotnie inicjalizacja naśladowała składnię obowiązującą w języku C, a wariant z klamrami wprowadzono w wersji C++11 w celu ujednolicenia składni: za pomocą tej składni można zainicjalizować wszystko, co w ogóle da się zainicjalizować, i nie można jej pomylić z deklaracją funkcji, co z kolei ułatwia pisanie szablonów.  Tą składnia można inicjalizować nawet zwykłe liczby (np. `int x{7};` oznacza to samo, co `int x = 7`). To bogactwo składni C++ najczęściej nie powoduje kłopotów, czasami jednak można się nieźle zdziwić i długo szukać błędu.   

### Podsumowanie 

- Do inicjalizacji za pomocą list wartości służy konstruktor z pojedynczym argumentem typu `std::initializer_list<Typ>`.
- W C++ do inicjalizacji stosuje się 3 rodzaje składni: ze znakiem `=`, z listą argumentów konstruktora w nawiasach okrągłych lub w klamrach lub z listą wartości początkowych w klamrach
  - Żeby uniknąć chaosu na początkowym etapie nauki, w tym wykładzie unikam klamer, jednak to klamry uznawane są za najbardziej bezpieczny i pożądany sposób inicjalizacji w nowoczesnym C++ 