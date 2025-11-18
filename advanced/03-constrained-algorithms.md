### Algorytmy ograniczone (*Constrained algorithms*)

Algorytmy ograniczone to algorytmy, w których zastosowano wprowadzone do języka C++20 koncepty. Nie chcę tu wchodzić w szczegóły - koncepty to osobny temat - zaledwie je tu naszkicuję.

#### Łagodne wprowadzenie do ograniczeń, czyli konceptów

##### Interfejs `std::sort`

Spójrzmy na definicję funkcji [`std::sort`](https://en.cppreference.com/w/cpp/algorithm/sort.html) z biblioteki STL języka C++:

```c++
template< class RandomIt >
void sort( RandomIt first, RandomIt last );
```

W tej definicji `RandomIt` oznacza dowolna klasę. W powyższej deklaracji moglibyśmy tę nazwę zastąpić czymkolwiek, pisząc np. tak:

```c++  
template< class RandomIt >
void sort( X first, X last );
```

Dla kompilatora obie te deklaracje byłyby nierozróżnialne. Jednak wolimy tę pierwszą, gdyż nazwa `RandomIt` mówi nam, że oba argumenty funkcji `std::sort` powinny być iteratorami o dostępie swobodnym. problem w tym, że tę wiedzę mamy my, ale nie kompilator. Jeżeli tę funkcję wywołamy z argumentami niespełniającymi warunków definiujących iterator o dostępie swobodnym, to kompilator wygeneruje błąd wraz z  kilkudziesięcioma wierszami trudnych w interpretacji informacji diagnostycznych. Problem w tym, że definicja iteratora o dostępie swobodnym występuje w definicji standardu języka, ale nie w samym języku. Koncepty to rozszerzenie języka umożliwiające udostępnienie tej informacji kompilatorowi, czego konsekwencją są lepiej zrozumiałe, krótsze komunikaty diagnostyczne. 

##### Interfejs `std::ranges::sort`

Powyższe przemyślenia najlepiej jest zilustrować przykładem. Spójrzmy na deklarację funkcji [`sort`](https://en.cppreference.com/w/cpp/algorithm/ranges/sort.html) z biblioteki `std::ranges`:

```c++ 
template< std::random_access_iterator I, std::sentinel_for<I> S,
          class Comp = ranges::less, class Proj = std::identity >
requires std::sortable<I, Comp, Proj>
constexpr I
    sort( I first, S last, Comp comp = {}, Proj proj = {} );
```

Tak, jest dłuższa i nieco trudniejsza w czytaniu. Jednak przy pewnej wprawie to naprawdę nie jest *rocket science*! Powyższa deklaracja informuje m.in., że `sort` jest funkcją generowaną z szablonu sparametryzowanego 4 typami danych, z których dwa ostatnie mają wartości domyślne. Sama funkcja jest funkcją czteroargumentową, z których obowiązkowo trzeba podać dwa pierwsze, `first` i `last`.  Ponadto:

- Typ argumentu `first`, `I`, musi spełniać warunki ("koncept") `std::random_access_iterator`, czyli być iteratorem o dostępie swobodnym;
- Typ argumantu `last`, `S`, musi spełniać warunki ("koncept") `std::sentinel_for<I>`, czyli być wartownikiem (ang. *sentiel*) dla typu `I`. O wartownikach piszę w jednym z kolejnych podrozdziałów.
- Typy `I`, `Comp` oraz `Proj` spełniają łącznie warunek ("koncept") `std::sortable<I, Comp, Proj>`, który oznacza, że dany zakres, który chcemy uporządkować, można uporządkować, stosując funktor porównawczy `comp` i operacje typowe dla algorytmów sortujących typu quicksort.  

Powyższe oznacza, że w przypadku drugiej z powyższych deklaracji kompilator ma pełny zestaw informacji odnośnie warunków, jakie muszą spełniać typy danych użytych w jej wywołaniu. Te warunki to właśnie ograniczenia (*constraints*), jakie biblioteka nakłada na te typy.

##### Przykład różnicy w działaniu "starej" i "nowej" biblioteki 

Rozważmy prosty przykład. Spróbujmy napisać niekompilujący się kod związany z próbą sortowania elementów kontenera nieposiadającego iteratora o dostępie swobodnym funkcją`sort` wymagającą takiego iteratora:

```c++  
std::list li = {1, 5, 2, 6};
std::sort(begin(li), end(li));  // błąd!
```

Kompilator zgłosi błąd i wyświetli kilkadziesiąt wierszy komunikatów diagnostycznych. W przypadku kompilatorów gcc i clang wszystkie one sugerować będą, że błąd znajduje się w którymś z plików biblioteki standardowej, gdyż kompilator, rozwijając szablon funkcji, nie posiada informacji, na podstawie których można jednoznacznie określić, czy błąd znajduje się w implementacji szablonu, czy też w próbie użycia go z nieprawidłowymi parametrami. 

Podobną funkcjonalność w bibliotece *ranges* zapisuje się nastepująco:

```c++       
std::list v = {1, 5, 2, 6};
std::ranges::sort(begin(v), end(v));
```

Oczywiście ten kod też nie będzie się kompilował. Jednak możemy liczyć na bardziej precyzyjny komunikat diagnostyczny. W przypadku kompilatora clang 20.1.8 zawiera on m.in. następujący opis wykrytego błędu (tu lekko skrócony):

```txt 
constrained.cpp:7:5: error: no matching function for call to object of type 'const __sort_fn'
    7 |     std::ranges::sort(begin(v), end(v));
      |     ^~~~~~~~~~~~~~~~~
note: candidate template ignored: constraints not satisfied
```

Czyli od razu dostajemy informację, że błąd jest w kodzie użytkownika (plik `constrained.cpp` wiersz 7, kolumna 5) i że jego powodem jest niespełnienie ograniczeń (konceptów) opisanych w dalszej części komunikatu diagnostycznego.

#### W bibliotece *ranges* algorytmy mogą operować na całych kontenerach 

Jednym z prostych, a jednocześnie bardzo użytecznym rozszerzeniem funkcjonalności biblioteki ranges w stosunku do klasycznej biblioteki STL jest możliwość używania praktycznie każdego algorytmu bezpośrednio na całych kontenerach:

```c++
std::vector v = {1, 5, 2, 6};
std::ranges::sort(v);
```

Nie wymaga to włączania żadnego nowego pliku nagłówkowego (makroinstrukcją `#include`) ani dołączania do programu nowych bibliotek. Ta część `ranges` została włączona po prostu do standardowego pliku `<algorithm>`, który funkcjonuje w C++ od wersji C++98. 

#### Przestrzeń nazw `std::ranges`

Jedyne, o czym musimy pamiętać, to że implementacja biblioteki `ranges` znajduje się w przestrzeni nazw `std::ranges`. Obok siebie można więc używać "starej" i nowej wersji biblioteki standardowej C++, przy czym "nowa" biblioteka udostępnia zarówno tradycyjny (oparty na parze iteratorów), jak i "nowoczesny" interfejs: 

```c++
std::vector v = {1, 5, 2, 6};
std::sort(v.begin(), v.end());  // Wersja STL obecna od C++98, najstarszy zapis
std::sort(begin(v), end(v));    // Jak wyżej, nieco nowszy zapis (od C++11)
std::ranges::sort(v);           // Biblioteka ranges, od C++20
std::ranges::sort(v.begin(), v.end());  // OK, dla miłośników tradycyjnego inerfejsu
std::ranges::sort(begin(v), end(v));    // OK, jak wyżej
```

#### Podsumowanie

- Począwszy od wersji języka C++20 praktycznie wszystkie wywołania funkcji ze standardowego pliku nagłówkowego `<algorithm>` możesz zastąpić podobnymi wywołaniami, w których zamiast przestrzeni nazw `std` występuje przestrzeń nazw `std::ranges`.
  - To bardzo prosta zmiana niewymagająca wielu godzin nauki. 
- Jeżeli operujesz na całych kontenerach, to przechodząc na bibliotekę *ranges*, możesz pomjać `begin` i `end`, przekazując od razu całe kontenery.
- Uzyskujesz w ten sposób:
  - bardziej zwarty, mniej podatny na błędy, łatwiejszy w lekturze kod
  - zdecydowanie bardziej czytelne komunikaty diagnostyczne w przypadku błędnego użycia szablonów z `<algorithm>`.
