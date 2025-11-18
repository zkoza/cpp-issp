### Zakresy (*ranges*) i ich widoki (*ranges::views*)

#### Zakresy

Przedstawione w poprzednim rozdziale algorytmy ograniczone (*constrained algorithms*) stanowią  ważny krok w rozwoju projektu biblioteki standardowej C++, jednak dla zwykłego użytkownika przełomem nie są. Gdyby ograniczyć się tylko do nich, to można by pisać nieco bardziej zwięzły i łatwiejszy w diagnostyce kod, jednak trudno uznać to za przełom. Sytuację zmieniają kolejne elementy biblioteki *ranges*, w tym zakresy i widoki zakresów.

Idea zakresów i widoków jest banalnie prosta. Zastanówmy się nad instrukcją z poprzedniego rozdziału:

```c++ 
std::ranges::sort(v);
```

Wiemy, że punktem wyjścia dla biblioteki *ranges* jest sytuacja, w której w powyższym wyrażeniu `v` jest kontenerem, czyli strukturą danych, dla której zdefiniowano `begin(v)` i `end(v)` jako iteratory do pierwszego elementu kontenera i do miejsca "tuż za" ostatnim elementem `v`, odpowiednio. No dobrze, ale czy musimy ograniczać się do kontenerów? Jaka jest najbardziej ogólna cecha obiektu `v`, która umożliwia wykonanie powyższej instrukcji? Są nią dwa związane z nim "obiekty", umownie nazwane `begin` i `end`, definiujące początek i koniec zakresu danych. Te dane nie muszą należeć do kontenera - na przykład mogą być generowane z jakiegoś wzoru. Bez trudu można przecież napisać klasę dla obiektu `v` tak, by np. pętla 

```c++
for (auto n: v) { rób coś }
```

generowała kolejne liczby parzyste. Ważne jest jednak, by zaczynając od `begin` i wykonując operację `++`, można było dojść do `end` sygnalizującego "koniec zakresu".  I tu mamy kolejne pytanie - czy `begin` i `end` muszą być tych samych typów, tak jak zakłada się w tradycyjnym projekcie biblioteki STL? Skoro `v` nie musi być kontenerem z danymi, to może `end` nie musi być tego samego typu, co `begin` (zwłaszcza że nie potrzebujemy, a nawet nie chcemy na nim definiować operacji `++` i `*`), choć oczywiście powinniśmy móc sprawdzić, czy `begin == end`, aby przy przeglądaniu zakresu element po elemencie móc określić, gdzie on się kończy.

Ostatecznie dochodzimy do wniosku, że **zakres** zdefiniowany jest przez parę obiektów `{begin, end}` i operacje `++`, `*` oraz `==`. Innymi słowy, zakres w C++ jest obiektem, `z`, takim, że 

- poprawne jest wyrażenie `auto it = z.begin()` lub `auto it = std::begin(z)`,
- poprawne jest wyrażenie  `auto se = z.end()` lub `auto se = std::end(z)`,
- poprawne są wyrażenia `++it`, `*it` i `it == se`
- wyrażenie `it == se` zwraca `true` wtedy i tylko wtedy, gdy zakres pomiędzy iteratorem `it` i wartownikiem `se` jest pusty. 

#### Widoki

Oczywistymi przykładami zakresów są kontenery biblioteki STL. Biblioteka *ranges* rozszerza jednak zbiór standardowych zakresów o **widoki** (*views*), będące "lekkimi" opakowaniami dla pary `{begin, end}`. Słowo "lekki" oznacza tu, że zajmują mało pamięci (nie przechowują danych, lecz jedynie stowarzyszony z nimi iterator i wartownika), a tworzenie, destrukcja, kopiowanie i modyfikacja widoków jest bardzo tania i niezależna od ilości i rodzaju danych objętych zakresem (w tym sensie są to właśnie "widoki" danych). Co więcej, ich modyfikacje zwykle mogą być wykonywane już podczas kompilacji dzięki optymalizacjom w rodzaju funkcji `constexpr` - w takim przypadku ich użycie nie kosztuje nic.

##### Przykładowy widok

Oto prosty przykład użycia widoku zakresu (*range view*) :

```c++
#include <print>
#include <ranges>
#include <vector>

int main()
{
    std::vector<int> v = {1, 3, 2, 4, -5}; // kontener
    std::ranges::take_view view(v, 3);     // widok kontenera
    for (auto x: view)
    {
        std::print("{} ", x);
    }
}
```

Program ten wyświetla 3 pierwsze elementy kontenera `v`:

```txt  
1 3 2
```

Wszystkie (standardowe) widoki znajdują się w przestrzeni nazw `std::ranges` i wymagają dołączenia do programu pliku nagłówkowego `<ranges>`, czyli makroinstrukcji  `#include <ranges>`. W powyższym przykładzie mamy do czynienia z jednym z najprostszych widoków: `take_view`. Jego działanie jest proste: zamienia on zakres `{begin, end}` w zakres `{begin, min(begin+3, end)}`. Innymi słowy, ogranicza zakres do maksymalnie 3 jego elementów. Nie trudno tez zauważyć, że działanie tego widoku jest bardzo proste, żeby nie powiedzieć: trywialne. Tę cechę posiada większość widoków z biblioteki *ranges*.

#### Adaptery zakresów i operator `|`

Przedstawione w poprzednim podrozdziale widoki nie są specjalnie często wykorzystywane jako osobne, nazwane obiekty. Znacznie częściej korzysta się z nich niejawnie, korzystając z tzw. adapterów zakresów. Rozpatrzmy prosty przykład kodu z poprzedniego podrozdziału, zapisanego jednak za pomocą adapterów:

```c++   
include <vector>

int main()
{
    std::vector<int> v = {1, 3, 2, 4, -5};
    for (auto x: v | std::ranges::views::take(3)) 
    {
        std::print("{} ", x);
    }
}
```

Charakterystyczną cechą powyższego kodu jest brak jawnej definicji obiektu klasy `std::take_view` oraz użycie przeciążonego operatora `|`. Otóż to ten operator tworzy w tym kodzie (nienazwany) obiekt klasy `std::take_view`. Innymi słowy, fragment powyższego programu można by zapisać i tak:

```c++ 
std::ranges::take_view view = v | std::ranges::views::take(3);
for (auto x : view)
{
    std::print("{} ", x);
}
```

chociaż wersja poprzednia jest chyba bardziej zrozumiała, a na pewno - bardziej powszechna. 

Bardzo charakterystyczną cechą adapterów zakresów jest to, że można je łączyć przedstawionym powyżej operatorem `|`. Można więc napisać następującą pętlę:

```c++  
for (auto x : v | std::ranges::views::take(3) | std::ranges::views::reverse)
{
    std::print("{} ", x);
}
```

W pętli tej ograniczamy zakres danych odczytywanych z `v` do maksymalnie 3 elementów, a następnie zmieniamy kierunek ich przeglądania, otrzymując w wyniku napis `2 3 1 `. 

Oczywiście pisanie pełnych, kwalifikowanych nazw adapterów zakresów, czyli podawanie wszystkich przestrzeni nazw, w których się znajdują, jest uciążliwe. Biblioteka *ranges* nieco łagodzi ten problem, pozwalając pomijać `ranges` w łańcuchu `std::ranges::views`, co redukuje przestrzeń nazw adapterów zakresów do `std::views`. Dlatego powyższą pętlę można skrócić do 

```c++
for (auto x : v | std::views::take(3) | std::views::reverse)
```

Oczywiście zapis można jeszcze bardziej uprościć, np. stosując instrukcję 

```c++ 
using namespace std::views;
```

W tym przypadku powyższą pętlę można zredukować do czytelnego zapisu

```c++ 
for (auto x : v | take(3) | reverse)
```

Generalnie, składnia używana w tym przykładzie, została zainspirowana przekierowaniami (*pipes*) w systemach UNIX, które realizowane są tam za pomocą symbolu `|`.  Zapis

```Oznacza, 
range | view_adaptor_1 | view_adaptor_2 | ...  
```

można rozumieć w ten sposób, że bierzemy zakres `range`, stosujemy na nim adapter `view_adaptor_1`, na wyniku tej operacji stosujemy adapter `view_adaptor_2` i tak dalej.  

#### Przykłady adapterów zakresu

Pełna lista adapterów zakresów dostępna jest m.in. na stronie https://en.cppreference.com/w/cpp/ranges.html. Zawiera ona tyle pozycji, że omawianie ich to temat na osobny wykład. Tu przedstawiam kilka najprostszych i, prawdopodobnie, najbardziej użytecznych.

- `filter` - widok, który pozwala uwzględnić w zakresie tylko te elementy, które spełniają jakiś warunek. Innymi słowy, ten adapter zakresu służy do odfiltrowywania niechcianych elementów zakresu. 

  Na przykład pętla

  ```c++ 
  for (auto x : v | std::views::filter([](auto n ){ return n % 2 != 0;}))
  ```

  pomija w zakresie `v` wszystkie liczby parzyste.

- `transform` - widok, który na każdym elemencie oryginalnego zakresu wykonuje pewne przekształcenie (transformację), nie modyfikując przy tym wartości w oryginalnym zakresie.

  Przykład:

  ```c++ 
  std::vector<int> v = {1, 2, 3, 4};
  for (auto x : v | std::views::transform([](auto n ){ return n * n ;}))
  {
      std::print("{} ", x);
  }
  std::println("\n{}", v);
  ```

  wyświetla

  ```txt
  1 4 9 16 
  [1, 2, 3, 4]
  ```

- `take(N)` - widok, w którym pomija się elementy zakresu poza jego pierwszymi `N` elementami.
- `drop(N)` - widok, w którym pomija się elementy zakresu poza jego ostatnimi `N` elementami.  

- `reverse` - widok, w którym kierunek przeglądania elementów ulega odwróceniu

- `zip_view" - widok "suwakowy", w którym dwa (lub więcej!) zakresy łączy się w zakres obejmujący pary kolejnych elementów z każdego z nich. Na przykład ten fragment programu

  ```c++ 
  std::vector v = {1, 2, 3, 4};
  std::list li = {"ojciec", "ręce", "liście", "ćwiartki"};
  for (auto [n, s] : std::views::zip(v, li))
  {
      std::println("{}: {} ", n, s);
  }
  ```

  wyświetla

  ```txt
  1: ojciec 
  2: ręce 
  3: liście 
  4: ćwiartki 
  ```

#### Efektywność kodu

Można odnieść wrażenie, że wyrażenia w rodzaju

```c++
for (auto x : v | take(3) | reverse)
{
    rób_coś(x);
}
```

wprowadzają do obliczeń dodatkowy narzut, niepotrzebne operacje. Przecież wyznaczenie wartości wyrażenia `v | take(3) | reverse` musi kosztować, prawda? Cały urok adapterów zakresów polega na tym, że dzięki ich prostocie wyrażenia, w których występują, w większości przypadków mogą być opracowane już podczas kompilacji. Mogą więc nieco wydłużyć czas kompilacji, ale zwykle nie wpływają na czas wykonania programu. Powyższa pętla powina więc wykonać się równie sprawnie, co jej napisany ręcznie odpowiednik

```c++ 
int k = std::min(v.size(), 3) - 1;
for (int i = k; i >= 0; i--)
{
    rób_coś(v[i]);
}
```

Wersja wykorzystująca *ranges* jest jednak, musimy przyznać, bardziej czytelna, a przez to łatwiejsza w utrzymaniu i bardziej odporna na błędy. 

Nie jest to jednak uniwersalna cecha biblioteki *ranges*. W Internecie można łatwo znaleźć przykłady złożonych widoków zakresów, z którymi współczesne kompilatory radzą sobie niespecjalnie dobrze. Jednym z przykładów jest kod, mający utworzyć widok napisu `s`, w którym pominięto wszystkie spacje znajdujące się na początku lub końcu tego napisu:

```c++
s | std::views::drop_while(is_space) 
                    | std::views::reverse 
                    | std::views::drop_while(is_space) 
                    | std::views::reverse;
```

gdzie `is_space` może być zdefiniowane tak:

```c++
constexpr bool is_space(const char c) noexcept {
  return c == ' ';
}
```

Ręczne utworzenie takiego zakresu zajmuje znacznie mniej instrukcji asemblera. Jeszcze gorzej wypadają testy wielokrotnego użycia adapterów w rodzaju `filter`. Z tego powodu dopóki nie pojawią się lepsze optymalizatory kodu, a zależy nam na wydajności, przed użyciem wersji z biblioteką `ranges` lepiej jest porównać wydajność tego kodu z kodem "tradycyjnym". 

#### Dalsza lektura

- https://learn.microsoft.com/en-us/cpp/standard-library/range-adaptors?view=msvc-170    (C++20)
- https://hackingcpp.com/cpp/std/range_views_intro.html
- https://en.cppreference.com/w/cpp/ranges.html

i wiele innych. Uwaga. Cześć bardzo dobrych materiałów dotyczy adapterów widoków w C++20 - takie materiały są już, wobec zmian wprowadzonych w późniejszych wersjach języka, niekompletne.   
