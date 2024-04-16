## Iterator

W wielu językach mamy proste pętle przebiegające całe kontenery (=zbiory danych). Na przykład w Javie mamy specjalną pętlę, której można użyć do przejrzenia zawartości tablicy:

```java
String[] samochody = {"Polonez", "Warszawa", "Syrena"};
for (String s : samochody) {
  System.out.println(s);
}
```

W C++ od pewnego czasu mamy podobną pętlę (zwaną czasem "pętlą w for w stylu Javy", choć oficjalnie jest to "*Range-based for loop*"):

```c++ 
std::vector samochody = {"Polonez", "Warszawa", "Syrena"};
for (const auto & s : samochody) 
{
    std::cout << s << "\n";
}
```

Jeśli jednak spróbujemy skompilować prosty program testowy dla naszej klasy `Wektor`,

```c++
int main()
{
    Wektor v = {1, 2, 4, 8, 16};
    for (auto n: v)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";
}
```

to kompilator zgłosi całą serię błędów zaczynających się np. tak:

```txt  
main10.cpp: In function ‘int main()’:
main10.cpp:8:18: error: ‘begin’ was not declared in this scope
    8 |     for (auto n: v)
      |                  ^
```

i podobnie dla innej brakującej funkcji, `end`. 

Przyczyna leży w tym, że pętli tego rodzaju nie można wykonywać na obiekcie dowolnego typu. W Javie to muszą być tablice (`arrays`). W Pythonie pętlę for można wykonać na listach, napisach (`string`) i zakresach (`range`).  Pod tym względem język C++ jest znacznie bardziej elastyczny, dostarcza bowiem mechanizmy umożliwiające kompilatorowi potraktowanie obiektów każdej odpowiednio przygotowanej klasy użytkownika jak kontenera, który można przeglądać pętlą `for` "w stylu języka Java".  W tym celu klasa musi udostępnić kompilatorowi specjalny interfejs - dwie bezargumentowe funkcje składowe o standardowych nazwach `begin` i `end`. Pierwsza z nich sygnalizuje, gdzie znajduje się pierwszy element kontenera, a druga - że cały kontener został już przejrzany. Obie te funkcje muszą zwracać obiekty specjalnej klasy, której definicje również musi dostarczyć użytkownik. Klasa ta zwyczajowo zwie się `iterator`, a jej obiekty służą do przeglądania zawartości kontenera. Oznacza to, że muszą one mieć dostęp do iterowanego (="przeglądanego po kolei") przez siebie kontenera oraz wewnętrzny stan informujący, które elementy zostały już przejrzane i który element będzie przejrzany jako następny. Iterator musi mieć też jakąś metodę określania kolejności elementów w kontenerze tak, by zawsze przeglądać je w tej samej kolejności, by żadnego elementu nie pominąć i by nigdy nie wrócić do raz przejrzanego elementu. Ponadto iterator umożliwia zwrócenie na zewnątrz wartości przeglądanego w danej chwili elementu iterowanego kontenera.          

Dość teorii. Czas na implementację. Ponieważ iteratory są ściśle zintegrowane z iterowanymi za ich pomocą obiektami, definicje ich klas zwykle umieszcza się wewnątrz definicji klasy obiektów. Taką strukturę nazywamy zagnieżdżeniem lub zanurzaniem (*embedding*) klasy w klasie. Początek definicji klasy `Wektor` może wyglądać następująco:

```c++ 
class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

  public:
    class iterator   // klasa w klasie! 
    {
        int cur_index = 0;
        const Wektor& wektor;

      public:
        iterator(const Wektor& w, int pos = 0)
            : cur_index{pos},
              wektor{w}
        {
        }
        int operator*() const { return wektor[cur_index]; }
        void operator++() { cur_index++; }
        bool operator!=(const iterator& rhs) const { return cur_index != rhs.cur_index; }
    };
   ...
}
```

Jak widać, definicja klasy `iterator` umieszczona jest całkowicie wewnątrz klasy `Wektor`. Posiada ona dwuargumentowy konstruktor i trzy przeciążone  operatory: `*`, `++` i `!=`, a także dwie składowe prywatne: liczbę całkowitą `cur_index` ("bieżący indeks") i stałą referencję do iterowanego obiektu, `wektor`. 

- Zgodnie z regułami sztuki, wszystkie dane w klasie `iterator` zostały ukryte w jej sekcji prywatnej.

- Jedną ze składowych każdego obiektu klasy `iterator` jest stała referencja do obiektu innej klasy (tu: `Wektor`). Inicjalizacja referencji możliwa jest wyłącznie na liście inicjalizacyjnej konstruktora (po dwukropku, tu: `wektor{w}`). Dzięki takiej składowej `iterator` może swobodnie czytać dane z klasy, z którą jest stowarzyszony.

- Składowa `cur_index` to liczba określająca, który element kontenera `Wektor` jest aktualnie przeglądany prze dany iterator

- `operator*` zwraca bieżącą wartość wskazywaną przez iterator w obiekcie `wektor`.

- `operator++` przesuwa iterator do kolejnego elementu kontenera

- `operator!=` pozwala określić, czy dwa iteratory mają taką samą wartość. Zakładamy, że zawsze porównuje się iteratory do tego samego obiektu.

- Wszystkie składowe iteratora znajdują się w przestrzeni naw `Wektor::iterator`. Chcąc zdefiniować np. `operator*` na zewnątrz klasy, należałoby tam więc definiować funkcję o sygnaturze  

  ```c++
  int Wektor::iterator::operator*() const;
  ```

Mając tak zaprojektowaną klasę `iterator`, możemy już zaimplementować  brakujące składowe `begin` i `end`. W deklaracji klasy `Wektor` (plik `vector.h`) dodajemy dwie deklaracje:

```c++ 
iterator begin() const;
iterator end() const;
```

Ich definicje możemy umieścić w pliku źródłowym:

```c++
Wektor::iterator Wektor::begin() const
{
    return iterator(*this, 0);
}

Wektor::iterator Wektor::end() const
{
    return iterator(*this, size());
}
```

Jak widać, oba iteratory, `begin` i `end`, odnoszą się do iterowanego (stała referencja do `*this`), różnią się zaś wartością indeksu: `begin()` ma indeks `0`, a indeks przechowywany w` end` równy jest rozmiarowi tablicy, czyli najmniejszemu indeksowi wskazującemu element poza tablicą. 

Po zdefiniowaniu powyższych funkcji, kompilator skompiluje pętlę w rodzaju

```c++  
for (auto n: v)
{
    std::cout << n << " ";
}
```

tak, jakby zapisano ją w postaci, którą używano do pojawienia się C++11:  

```c++
for (auto iter = v.begin(); iter != v.end(); ++iter)
{
    std::cout << *iter << " ";
}
```

Pętla rozpoczyna się od zdefiniowania obiektu (iteratora) `iter` i nadanie ma wartości początkowej zwróconą przez składową `begin()` kontenera. Wartości wskazywane przez iterator wyłuskiwane są w treści pętli poprzez `operator*` (tu: `*iter`). Na zakończenie każdego przebiegu pętli iterator przesuwany jest do kolejnego elementu  operatorem przedrostkowym `++` (tu: `++iter`). Pętla kończy się w chwili, gdy iterator osiągnie wartość `v.end()`. Ta ostatnia właściwość stanowi w zasadzie definicję funkcji `end()`: zwraca ona iterator w stanie generowanym przez `operator++` na ostatnim elemencie (niepustego) kontenera lub `begin()`, jeśli kontener jest pusty.

Jeśli powyższe dywagacje wydają się zbyt abstrakcyjne, proszę wrócić do tej lektury po opanowaniu działu "iteratory".

### Uwagi

Przedstawiona powyżej implementacja jest niepełna, gdyż nie umożliwia skompilowania pętli modyfikującej kontener, np.:

 ```c++
 for (auto & x : Wektor)
     x = 0;
 ```

przy czym komunikat kompilator może być tu wyjątkowo mało pomocny, np. "*cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’*". Chodzi o to, że w naszym iteratorze składowa `opertor*` ma modyfikator `const`. Można ten problem rozwiązać, definiując dwie klasy iteratorów: zwykły `iterator` i `const_iterator`.  Trzeba też do klasy `Wektor` dodać dwie dodatkowe funkcje składowe `begin()` i `end()`  nieposiadające atrybutu `const` i zwracające zwykły `iterator`, podczas gdy tak samo nazwane funkcje z atrybutem `const` zwracać będą `const_iterator`. Ponadto `operator*`  w `iterator` musi zwracać referencję do obiektu, na który wskazuje.

Kompletna implementacja uwzględniająca te pomysły:

- [vector10.h](././cpp/wektor/vector10.h)
- [vector10.cpp](./cpp/wektor/vector10.cpp)
- [main10.cpp](./cpp/wektor/main10.cpp)

### Podsumowanie

Aby pętla w rodzaju 

```c++   
for (auto n: kontener) { rób_coś(); }
```

działała w C++:

- klasa kontenera musi definiować swoją klasę iteratorów oraz składowe `begin` i `end` 
- klasa `iterator` musi definiować `operator*`, `opeartor++()` i `operator!=` 
- aby dodatkowo w takiej pętli można było modyfikować elementy kontenera, zmienna sterująca w pętli musi być definiowana przez referencję, np. 
  ```c++ 
  for (auto & n: kontener) { rób_coś(n); }
  ```

  a klasa kontenera musi mieć dwie funkcje składowe `begin` i `end`  (stałą i niestałą) oraz zawierać podklasę `iterator` i `const_iterator`.  
- iteratory służą do przeglądania kontenerów od pierwszego do ostatniego elementu, a towarzysząca im składnia wzorowana jest na składni arytmetyki na wskaźnikach  

Pisanie własnych klas iteratorów nie jest najbardziej poszukiwaną umiejętnością u programistów C++, jednak dobrze ilustruje sposób myślenia o programowaniu charakterystyczny dla tego języka, pozwala też wprowadzić nowe koncepcje języka, takie jak zagnieżdżanie definicji klas.