### Wartownicy (*Sentinels*)

W informatyce [wartownik](https://en.wikipedia.org/wiki/Sentinel_value) (ang. *sentinel*) to specjalna wartość sygnalizująca koniec jakiegoś strumienia danych. Tego rodzaju rolę pełnią m.in.:

- Znak `'\0'` jako znacznik końca napisu w stylu języka C, np. `"Ala ma kota"`;  
- `nullptr` jako znacznik końca listy linkowanej
- Znak `'\n'` jako znacznik końca wiersza
- Znak `','` jako znacznik końca rekordu danych w standardowym formacie CSV
- Znak EOF jako znacznik końca pliku
- Znak NULL (`nullptr`) jako znacznik końca tablicy argumentów wiersza poleceń w programach napisanych w C/C++ (ostatnim elementem `argv` jest zawsze `nullptr`) 

Wartownik pozwala wykryć koniec strumienia danych, jeżeli nie mamy innej metody detekcji tego zdarzenia, np. jawnej informacji o długości strumienia danych. Jego wartość musi być różna od każdej "poprawnej" wartości, jaka może pojawić się w strumieniu. Poza tym, jeżeli wartość zapisaną w strumieniu nazwiemy `x`, a wartość wartownika oznaczymy jako `s` , to musi być poprawne wyrażenie `if (x == s)`.  

Często wartownik jest tego samego typu co dane, które "pilnuje". Tak jest np. ze znakiem końca wiersza, `'\n'`. Jednak już w przypadku `nullptr` jako znacznika końca listy linkowanej, jego typ (`std::nullptr_t`) jest różny od typu danych ("wskaźnik na coś"), co jest bardzo wygodne. W bibliotece STL ze standardu C++98 przyjęto zasadę, że wartownik musi być tego samego typu co każdy iterator i w dodatku musi być związany z tym samym kontenerem, co iterator. \W bibliotece *ranges* przyjęto jednak ogólniejszą zasadę, że typ wartownika może, ale nie musi być tożsamy z typem iteratora, co znacznie upraszcza jego implementację i użycie. Praktycznie jedyne wymaganie dotyczy tego, że musi istnieć możliwość porównania iteratora `it`   z wartownikiem `se` operatorem `==`, czyli poprawne musi być wyrażenie `it == se`.

#### Przykład

Oto przykład wartownika, który kończy ciąg liczb całkowitych w momencie, w którym pojawi się w nim liczba ujemna:

```c++ 
#include <algorithm>
#include <print>
#include <vector>

struct Ujemna
{
  bool operator==(auto iterator) const
  {
    return *iterator < 0;
  }
};

int main()
{
   std::vector v = {1, 2, 3, 6, -1, 7, 1};
   auto lambda = [](auto n){ std::print("{} ", n); };
   std::ranges::for_each (v.begin(), Ujemna(), lambda);     
}
```

Program ten wyświetla na standardowym wyjściu napis  `1 2 3 6 `.

Alternatywna, chyba jeszcze prostsza implementacja oparta na typie pustym i przeciążonym dla niego operatorze `==`:

```c++
#include <algorithm>
#include <print>
#include <vector>

struct Ujemna
{
};

bool operator==(Ujemna u, auto iterator)
{
    return *iterator < 0;
}

int main()
{
   std::vector v = {1, 2, 3, 6, -1, 7, 1};
   auto lambda = [](auto n){ std::print("{} ", n); };
   std::ranges::for_each (v.begin(), Ujemna(), lambda);     
}
```

W powyższym rozwiązaniu można zdefiniować zarówno `operator==(Ujemna u, auto iterator)`, jak i   `operator==(auto iterator, Ujemna u)`, oba będą działały równie dobrze. Wynika to z tego, że począwszy od C++20, jeżeli masz dwa typy `A` i `B` i dwa obiekty `a` i `b` typu `A` i `B` odpowiednio, to jeżeli zdefiniujesz znaczenie wyrażenia `a == b`, to kompilator sam wygeneruje trzy brakujące (i oczywiste w implementacji) dodatkowe operatory przypisania tak, by prawidłowe były wyrażenia `b = a`, `a != b` i `b != a`.    

#### Czy wartownik może być lambdą?

Nie. Wartownik w bibliotece *ranges* nie może być lambdą. Lambda dostarcza przeciążony operator wywołania funkcji, `operator()`, natomiast wartownik musi dostarczać `operator==`. 

Uwaga na marginesie. Skro w STL koniec zakresu weryfikuje się wartością logiczną wyrażenia  `begin != end` lub `begin == end`, to w bibliotece *ranges*, która jest zgodna z STL, jednym z argumentów operatora `==` jest wartownik, a drugim - iterator a nie wartość odczytywana tym iteratorem z zakresu. Wartownik (*sentinel*) jest uogólnieniem iteratora a nie wartości. Dzięki temu wartownik nie narzuca żadnych ograniczeń na wartości przechowywane w zakresie.

#### Podsumowanie

Koncepcja wartownika pozwala znacznie rozszerzyć pojęcie zakresu oraz ułatwić jego (=zakresu) implementację. Dzięki wartownikom możliwe jest m.in. 

- Definiowanie dynamicznych warunków końca zakresu. Np. zakres może kończyć się z chwilą, gdy suma jego elementów osiągnie określoną wartość.
- Używanie algorytmów z biblioteki *ranges* na danych nieposiadających iteratora `end()`, np. na napisach w stylu języka C ("Ala ma kota"), tablicach, do których dostęp zapewnia wskaźnik, lub obiektach z bibliotek, które nie zapewniają `end()`, a których modyfikować czy nadpisywać nie chcemy.   
- Stosowanie algorytmów z biblioteki *ranges* do strumieni danych.
- Znaczne uproszczenie implementacji funkcji `end()`.