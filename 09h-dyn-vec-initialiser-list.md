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

Korzystamy to z tego, że lista inicjalizacyjna posuada składową `size` zwracającą liczbę elementów listy. Używamy jej do inicjalizacji rozmiaru logicznego (`_size`) i fizycznego (`_capacity`) listy. 

​      