### Przykład użycia funkcji  `qsort` 

Rozpatrzmy dość zaawansowany przykład użycia wskaźników, przygotowany na podstawie przykładu z serwisu [cppreference.com](https://en.cppreference.com/w/c/algorithm/qsort) (oryginalny program napisany jest w czystym C, tu  pozwoliłem sobie go nieco uprościć i dostosować do C++). Ilustruje on sposób użycia funkcji `qsort` do sortowania tablicy zmiennych typu `int` :

```c++
#include <iostream>
 
int compare_ints(const void* a, const void* b)       // (a)
{
    int arg1 = *(const int*)a;                       // (b)
    int arg2 = *(const int*)b;                       // (c)
 
    if (arg1 < arg2) return -1;                      // (d)
    if (arg1 > arg2) return 1;                       // (e)
    return 0;                                        // (f) 
}
 
int main(void)
{
    const int size = 7;                               // (1) 
    int ints[size] = { -2, 99, 0, -743, 2, -77, 4 };  // (2) 
 
    qsort(ints, size, sizeof(int), compare_ints);     // (3)
 
    for (int i = 0; i < size; i++)                    // (4)
    {
        std::cout << ints[i] << " ";                  // (5)
    }    
    std::cout << "\n";                                // (6)
}
```

- W wierszach 1-2 definiujemy rozmiar tablicy (`size`) oraz tablicę (`ints`) wraz z jej wartościami początkowymi.

- W wierszu (3) wywołujemy funkcję `qsort`. Ma ona następujący prototyp:

  ```c++
  void qsort( void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *) ); 
  ```

  - Pierwszym argumentem tej funkcji jest wskaźnik na początek sortowanej tablicy. Twórcy funkcji `qsort` nie mogli przewidzieć, jakiego typu będą dane, które chcemy sortować, dlatego użyli typu `void*`. Można więc przekazać tu adres początku dowolnej tablicy dowolnego typu i dowolnej długości. Majć to na uwadze, w wierszu 3 jako pierwszego argumentu tej funkcji użyto `ints` . Wykorzystano tu informację, że nazwa tablicy może zostać automatycznie zrzutowana na wskaźnik do jej pierwszego elementu. Można tu też  było użyć wyrażenia `&ints[0]`. 

  - Drugim argumentem `qsort` jest liczba elementów tablicy. Dlatego wstawiono tu stałą `size`. Można też było wstawić tu literał `7`.  Typ tego argumentu to `size_t` - standardowy typ dla rozmiarów tablic. Jest to typ bez znaku (`unsigned`), a więc `size` nigdy nie jest ujemne.

  - Trzecim argumentem funkcji  `qsort` jest liczba bajtów zajmowanych przez każdy element tablicy. W powyższym przykładzie wykorzystano wyrażenie `sizeof(int) `. Można też było tu użyć wyrażenia `sizeof(*ints)` lub `sizeof(ints[0])`. Ten argument jest niezbędny, gdyż `qsort` nie wie, co sortuje. 

  - Czwarty argument `qsort` ma sygnaturę

    ```c++
    int (*comp)(const void *, const void *)
    ```

    która oznacza wskaźnik na... funkcję o sygnaturze 

    ```c++
    int dowolna_nazwa_funkcji(const void* a, const void* b);
    ```

    Chodzi tu o wskaźnik na funkcję dwuargumentową zwracającą `int`, przy czym oba argumenty tej funkcji są wskaźnikami przekazywanymi jako  `void*`.  Gdyby twórcy funkcji `qsort`  wpisali tu jakiś konkretny typ, np.  `int` - por. `int dowolna_nazwa_funkcji(const int* a, const int* b);`, to `qsort` mógłby sortować wyłącznie tablice elementów typu `int`.  Dzięki zastosowaniu `void*` można funkcją `qsort` porządkować (prawie) wszystko. Skąd wziąć wskaźnik ten na funkcję? Metoda jest prosta. Trzeba gdzieś w kodzie zdefiniować funkcję, która będzie używana przez `qsort`  (w naszym przykładzie jest to funkcja `compare_ints`), a następnie w wywołaniu funkcji `qsort` użyć jej nazwy. Z funkcjami jest bowiem podobnie do tablic: ich nazwa jest w wyrażeniach interpretowana jak wskaźnik (adres) na początek kodu funkcji. W ten sposób powinno być już jasne, skąd taka a nie inna postać instrukcji

    ```c++
    qsort(ints, size, sizeof(int), compare_ints); 
    ```

  - Prototyp funkcji `compare_ints`, czyli `int compare_ints(const void* a, const void* b)`, jest zgodny z prototypem funkcji oczekiwanej przez `qsort`:  `int dowolna_nazwa_funkcji(const void* a, const void* b);`. Implementacja tej funkcji może jednak sprawić kłopot, gdyż z jej deklaracji nie wynika, jaki jest rzeczywisty typ jej argumentów. W tym momencie robimy "sztuczkę" - kompilator nie wie, jaki jest ich typ, ale my wiemy i możemy tę informację przekazać kompilatorowi. Wiemy bowiem, że  `compare_ints` ma być używana wyłącznie do porównywania wartości typu `int`. Mówimy o tym kompilatorowi w wierszach b i c. Na przykład instrukcja

    ```c++
    int arg1 = *(const int*)a;
    ```

    oznacza, że tak naprawdę pierwszy argument funkcji `compare_ints` jest niemodyfikującym wskaźnikiem na `int`, czyli jest typu `const int*`. Stąd w powyższym wyrażeniu operator rzutowania, `(const int*)`, który w tym wyrażeniu zmienia typ `a` z `const void*` na `const int*`. Kolejna gwiazdka wyłuskuje z tego wskaźnika wartość argumentu. Jeżeli ktoś tego dalej nie rozumie, to może łatwiej mu pójdzie z jej rozpiską na dwie prostsze instrukcje:

    ```c++
    const int tmp_ptr = static_cast<const int*>(a);
    int arg1 = *tmp_ptr;
    ```

    Podobnie "wyciąga się" wartość drugiego argumentu `arg2` z wskaźnika `b` zadeklarowanego jako `const void* b`. Odtąd kod wykorzystuje wyłącznie zmienne `arg1` i `arg2`. 

    Funkcja używania do porównywania elementów sortowanej tablicy musi zwracać:

    - wartość ujemną, jeżeli `arg1` ma w posortowanej tablicy poprzedzać `arg2` (czyli jeżeli `arg1 < arg2`) 
    - wartość dodatnią, jeżeli  to `arg2` ma poprzedzać `arg1` (czyli jeżeli `arg1 > arg2`)
    - zero, jeżeli nie ma znaczenia, czy `arg1` ma występować przed czy po `arg2` (czyli `arg1` jest równoważne `arg2` ).