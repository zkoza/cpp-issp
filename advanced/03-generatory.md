### Generatory zakresów

#### Wstęp

Jak już wiemy, zakres w bibliotece *ranges* jest pojęciem bardzo ogólnym i nie musi odwoływać się do kontenera czy jakiegokolwiek zbioru danych. Alternatywą do kontenerów są m.in. generatory, czyli zakresy, w których kolejne wartości nie są sczytywane z kontenerów za pomocą iteratorów, lecz generowane, zwykle z poprzedniej wartości. 

Generatory znane są z wielu języków programowania. Na przykład w języku Python tworzy się je funkcją range:

```pyt  
>>> import sys
>>> a = range(1, 1000000)
>>> sys.getsizeof(a)
48
```

W powyższym kodzie `a` jest zakresem obejmującym wszystkie liczby całkowite od jeden do miliona, jednak obiekt ten zajmuje zaledwie 48 bajtów. Podobną rolę w języku Matlab/Octave pełni operator `:`,

```matlab
octave:1> a = 1:1000000;
octave:2> whos
Variables visible from the current scope:

variables in scope: top scope

  Attr   Name        Size                     Bytes  Class
  ====   ====        ====                     =====  ===== 
         a           1x1000000                   24  double

Total is 1000000 elements using 24 bytes
```

Jak widzimy, w tym przypadku do zapisania "100000 elementów" (czyli liczb od 1 do 100000) wystarczyły 24 bajty.

#### Genarator `iota`

Podstawowym generatorem włączonym do biblioteki standardowej C++ jest `std::ranges::views::iota`.  To, co go wyróżnia, to fakt, że służy od do definiowania zarówno zakresów o określonej długości, jak i nieskończonych. Oto przykład:

```c++ 
for (auto n : std::views::iota(0, 10))
    std::print("{} ", n);
std::println("");
for (auto n : std::views::iota(0) | std::views::take(10))
    std::print("{} ", n);   
```

W powyższym kodzie obie pętle wyświetlają ten sam tekst:

```txt
0 1 2 3 4 5 6 7 8 9 
0 1 2 3 4 5 6 7 8 9
```

Różnica polega na tym, że w pierwszej używamy `iota` do utworzenia zakresu skończonego, od 0, z krokiem 1, 10 kolejnych liczb, czyli 0, 1, 2, ..., 9. Natomiast w pętli drugiej tworzymy zakres nieskończony  obejmujący wszystkie liczby całkowite większe bądź równe 0 (wyrażenie `std::views::iota(0)`), po czym ograniczmy go adapterem zakresu, `std::views::take(10)`. 

Zasadniczo, jeżeli `std::views::iota` wywołamy z jednym argumentem (którego typem w praktyce musi być jeden z arytmetycznych typów całkowitych, a dokładniej - "integer-like type"), jest zakresem nieskończonym (pomijamy tu kwestie związane z reprezentacją maszynową liczb całkowitych i przepełnieniem całkowitoliczbowym, *integer overflow*). być Jeżeli zaś wywołanie będzie miało dwa argumenty, to otrzymamy zakres skończony, przy czym drugi argument będzie pełnił rolę ogranicznika czy tez wartownika, którego nie obejmuje zakres. Dlatego zarówno w wyrażeniu `std::views::iota(0, 10)`, jak i `std::views::iota(6, 10)`, ostatnią liczbą każdego z tych zakresów jest `9`.  Typem pierwszego argumentu tej funkcji nie może być  `bool` ani wskaźnik, ale poza tym może być dowolnym typem całkowitym (np. `int`, `unsigned long` itp. ).

W typowej implementacji, w której `sizeof(int) == 4`, wartość wyrażenia `std::views::iota(n, m))`, w którym `n` i `m` są typu `int`, mieści się na zaledwie 8 bajtach niezależnie od wartości `n` i `m`. Z kolei wyrażenie     `std::views::iota(n))` z reguły zajmuje w pamięci tylko 4 bajty (!).

#### Ciągi dowolne

Skoro mamy możliwość wygenerowania ciągu $n, n+1,\ldots$, to korzystając z adapterów zakresów możemy dość łatwo tworzyć generatory dowolnych ciągów.

##### Generator liczb parzystych

Zakres obejmujący wyłącznie ciąg 10 kolejnych liczb parzystych większych bądź równych 2 można zaimplementować następująco:

```c++  
using namespace std::views; // krótszy zapis
for (auto n : iota(1) | transform([](auto n){return 2*n;}) | take(10))
    std::print("{} ", n);  // 2 4 6 8 10 12 14 16 18 20
```

Jest jasne, że w ten sposób można generować dowolny ciąg $c_n$, w którym wartość kolejnego elementu, $c_n$, zależy wyłącznie od indeksu, $n$.

##### Generator ciągu Fibonacciego

Poniższy fragment programu przedstawia generator liczb Fibonacciego:

```c++
 using namespace std::views;
 auto fibo = [prev = 0, next = 1](int) mutable
 {
    auto sum = prev + next;
    prev = next;
    next = sum;
    return prev;
};
for (auto n : iota(0, 10) | transform(fibo))
    std::print("{} ", n); // 1 1 2 3 5 8 13 21 34 55
```

Warto w tym miejscu przypomnieć, że w powyższej definicji lambdy `prev` i `next` nie są jej argumentami, lecz składowymi, dlatego ich wartości są przechowywane między kolejnymi wywołaniami lambdy.   Deklarator `mutable` jest niezbędny do tego, by lambda mogła zmieniać wartości tych  składowych. Z kolei wykorzystany tu wzór rekurencyjny na n-tą liczbę Fibonacciego, $f_n = f_{n-1} + f_{n-2}$, nie zależy od $n$, dlatego argument lambdy nie ma nazwy (`(int)`) - nie jest ona do niczego potrzebna. Z tego powodu nie ma też znaczenia, czy użyjemy wyrażenia `iota(0, 10)` czy np. `iota(100, 110)`, czy nawet `iota(0) | transform(fibo) | take(10)`.

##### Generator liczb pierwszych

Generator liczb pierwszych można zbudować w następujący sposób:

```c++  
#include <print>
#include <ranges>

auto is_prime = [](int number) -> bool
{
  if (number <= 1) return false;      // 0 i 1 nie są pierwsze
  if (number <= 3) return true;       // 2 i 3 są liczbami pierwszymi
  if (number % 2 == 0) return false;   // wielokrotności 2 nie są pierwsze
  if (number % 3 == 0) return false;   // wielokrotności 3 nie są pierwsze

  // Teraz tylko liczby postaci 6k ± 1 mogą być dzielnikami 'number'
  for (int factor = 6; (factor - 1) * (factor - 1) <= number; factor += 6)
  {
    if (number % (factor - 1) == 0) return false;
    if (number % (factor + 1) == 0) return false;
  }
  return true;  // nie znaleziono dzielnika - liczba 'number' jest pierwsza 
};

int main()
{
  for (auto n : std::views::iota(1) |
                std::views::filter(is_prime) |
                std::views::take(20)
      )
  {
    std::print ("{} ", n);
  }
}
```

Program wyświetla ciąg 20 kolejnych liczb pierwszych od 2:

```txt
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
```





