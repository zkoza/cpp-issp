#### Projekcje (*projections*)

Przyjrzyjmy się uważnie kanonicznej pętli wykorzystującej zakres w bibliotece STL (C++98):

```c++
for (auto it = v.begin(); it != v.end(); ++it)
{
    auto value = *it;
    rób_coś(value);
}
```

Biblioteka *ranges* zmienia niemal wszystkie warunki nakładane na powyższy kod.

- `v` nie musi być kontenerem. Wystarczy, że jest zakresem. Zakresami są nie tylko kontenery, ale i generatory zakresów (ciągów), w tym generatory zakresów nieskończonych.
- `begin` nie musi być związane z `v`, tj. `v` nie musi być klasą; wystarczy, że działa `std::begin(v)` (i podobnie `std::end(v)`). Tę zmianę wprowadzono w C++11 u umożliwiła ona jednolite traktowanie kontenerów i tablic "w stylu języka C", czyli np. `int tab[7];`.
- `end()` nie tylko nie musi być związane z `v`, ale i nie musi być tego samego typu co `begin()`. Stąd w *ranges* pojawiło się pojęcie wartownika (ang. *sentinel*) i żądanie, by można go było porównywać z iteraorem operatorem `==`.
- `operator++`, działając na iteratorze, nie musi przeglądać kolejnych elementów zakresu. Jego działanie możemy zmienić, stosując odpowiedni adapter zakresu, np. `std::views::filter`, `std::views::reverse` itp.

Na tej liście pozostał jeden element, którego dotąd nie omawiałem, a który również został "uogólniony" w *ranges*: jest nim operator `*` (i, podobnie, `->`) działający na iteratorze. Dla porządku wypiszę tę zmianę w osobnym punkcie, po czym omówię ją w dalszej części tego rozdziału.

- `operator*` (i podobnie `operator->`), działając na iteratorze, zwraca wartość elementu zakresu zmodyfikowaną dowolną funkcją dostarczoną przez użytkownika. Ta (niemal) "dowolna funkcja" w kontekście biblioteki `ranges` to właśnie tytułowa projekcja.  

  #### Przykład

Omówienie projekcji najłatwiej zacząć od przykładu. 

```c++
#include <algorithm>
#include <print>

int main()
{
  std::pair<int, std::string> pairs[] = {{2, "foo"}, {1, "bar"}, {0, "baz"}};
  auto projection = [](auto const& p) { return p.first; }; 
  std::ranges::sort(pairs, std::ranges::less{}, projection);
  std::println("{}", pairs);
}
```

Program ten wyświetla na wyjściu

```txt
[(0, "baz"), (1, "bar"), (2, "foo")]
```

czyli pierwotny wektor uporządkowany względem wartości pierwszej składowej.

W programie tym algorytm `sort` przyjmuje dodatkowy, trzeci argument, którym właśnie jest projekcja, tu będąca lambdą o niewyszukanej nazwie `projection`. Lambda ta niejako podstawiana jest pod operator `*` w wyrażeniach, w których za pomocą iteratora zakresu uzyskujemy wartość należącą do tego zakresu. Innymi słowy, możemy sobie wyobrazić, że `operator* (Iterator it)` zamiast zwrócić wartość `value` wskazywaną przez `it`, zwraca `projection(it)`, gdzie `projection` to nazwa projekcji. Mam nadzieję, że to jasne. W powyższym przykładzie oznacza to, że `sort` wszędzie tam, gdzie ma do porównania dwa elementy z zakresu, powiedzmy, `a` i `b`, nie porównuje ich bezpośrednio, np. wyrażeniem `if (a < b)`. Po pierwsze, `sort` ma dostęp do elementów zakresu poprzez iteratory. Powiedzmy, że `a` wskazywane jest przez iterator `ia`,  a `b` - przez iterator `ib`. Przed biblioteką ranges, czyli np. w tradycyjnej implementacji biblioteki STL (C++98 i nowsze), funkcja sort robiła porównania w rodzaju `if (*ia < *ib)`. Biblioteka *Ranges* idzie dalej i powyższe porównanie zamienia na `if (projection(*ia) < projection(*ib))` - i robi tak konsekwentnie przy każdej próbie wyłuskania wartości za pomocą operatora `*` zastosowanego na iteratorze. Jak to jest dokładnie robione - nie sprawdzałem, ale wydaje mi się, że najłatwiej byłoby ten efekt osiągnąć, zastępując oryginalne iteratory iteratorami własnymi, które na oryginalną wartość zwracaną oryginalnym iteratorem nakładają "projekcję". 

Powyższy kod można nieco uprościć. Zamiast `std::ranges::sort(pairs, std::ranges::less{}, projection);` można napisać

```c++
std::ranges::sort(pairs, {}, projection);
```

W tym przypadku wyrażenie `{}` użyte w miejscu, w którym kompilator oczekuje jakiejś wartości, że kompilator zastąpi je wartością domyślną w danym typie. W przypadku szablonu `std::ranges::sort`, domyślną wartością drugiego argumentu tej funkcji jest `std::ranges::less{}`, czyli, poza kilkoma wyjątkami, `operator<`.

Wyrażenie to można jeszcze bardziej uprościć, zapisując lambdę bezpośrednio w wywołaniu funkcji:

```c++ 
std::ranges::sort(pairs, {}, [](auto const& p) { return p.first; });
```

#### Rodzaje projekcji

##### Projekcja poprzez lambdę, obiekt funkcyjny lub wskaźnik na funkcję

W przykładzie z poprzedniego punktu projekcja była wyrażeniem lambda, czyli obiektem funkcyjnym (obiektem z przeciążonym operatorem wywołania funkcji). Można się bez trudu domyślić, że zamiast obiektu funkcyjnego, w roli projekcji można użyć wskaźnika na funkcję. W tym przypadku powyższy przykład mógłby wyglądać tak:     

 ```c++   
 #include <algorithm>
 #include <print>
 
 int projection(const std::pair<int, std::string> & p) { return p.first; };
 
 int main()
 {
     std::pair<int, std::string> pairs[] = {{2, "foo"}, {1, "bar"}, {0, "baz"}};
     std::ranges::sort(pairs, std::ranges::less{}, projection);
     std::println("{}", pairs);
 }
 ```

Mogłoby się wydawać, że więcej możliwości już nie ma, że trzecim argumentem `std::ranges::sort` może być każde "coś", co można wywołać jak funkcję, czyli każdy obiekt `f` taki, że poprawne jest wyrażenie `f(argumenty...)`. Okazuje się jednak, że twórcy biblioteki poszli dalej: trzecim argumentem `sort` może być wszystko, co może być argumentem funkcji `std::invoke`, która co prawda jest sprytnym opakowaniem dla wywołań funkcji, ale nie tylko (por. jej [dokumentacja](https://en.cppreference.com/w/cpp/utility/functional/invoke.html)). Dzięki temu rozwiązaniu, trzecim argumentem `std::ranges::sort` oraz innych algorytmów z przestrzeni nazw `std::ranges` mogą też być wskaźniki na składowe klas. I to zarówno składowe funkcyjne ("metody", ang. *methods*), jak i z danymi ("właściwości", ang. *properties*). Najlepiej wyjaśnią to przykłady.

##### Projekcja poprzez wskaźnik na składową do funkcji składowej ("metody")

Rozpatrzmy następujący przykład:

```c++    
#include <algorithm>
#include <print>
#include <vector>

struct Para
{
    int nr = 0;
    std::string nazwa = "";
    auto proj() { return nr; }
};

int main()
{
    std::vector<Para> pairs = {{2, "foo"}, {1, "bar"}, {0, "baz"}};
    std::ranges::sort(pairs, std::ranges::less{}, &Para::proj);
    for (auto p : pairs)
    {
        std::print("({}, {}) ", p.nr, p.nazwa);
    }
}
```

Po uruchomieniu, program wyświetla wyjściową tablicę `pairs` uporządkowaną wg wartości zwracanych przez funkcję składową `Para::proj`:

```txt 
(0, baz) (1, bar) (2, foo)
```

W przykładzie tym projekcję wykonuje składowa `proj` klasy `Para`, która "podmienia" każdy obiekt klasy `Para`, nazwijmy go `p`, na wartość wyrażenia `p.proj()`. Projekcję definiujemy, jak zwykle, w trzecim argumencie funkcji `std::ranges::sort`. W tym przypadku odpowiada za to wyrażenie `&Para::proj`, które jest wskaźnikiem na funkcję składową `proj` klasy `Para`. 

Na marginesie: wskaźniki na składowe klas nie mają, poza nazwą i składnią opartą na symbolach `.`, `*` i `->`, wiele wspólnego ze zwykłymi wskaźnikami. W szczególności, nie przechowują adresów w pamięci operacyjnej i nie można ich użyć bez obiektu klasy, na której składową wskazują. Jeżeli zadeklarujemy p jako wskaźnik na składową

```c++  
auto p = &Para::proj;
```

i jeżeli mamy obiekt `para` klasy `Para`

```c++ 
Para para = {9, "Koszykówka"};
```

to możemy wywołać na nim funkcję składową poprzez wskaźnik na nią w następujący sposób: 

```c++
(para.*p)();;
```

np. pisząc ``` auto n =  (para.*p)();```. Nawiasy wokół `para.*p` są niezbędne, gdyż występujące po nim nawiasy okrągłe mają wysoki priorytet i wyrażenie `para.*p()` interpretowane by było, błędnie, jako `para.*(p())`. Innymi słowy, `p` traktowane byłoby nie jako wskaźnik na składową, lecz jako nazwa funkcji. Koniec dygresji. 

##### Projekcja poprzez wskaźnik na składową do składowej z danymi ("właściwości")

Przedstawiony w poprzednim podrozdziale przykład można jeszcze bardziej uprościć, pomijając w `Para` funkcję składową służącą za projektor i zastępując w wywołaniu `std::ranges::sort` wskaźnika na funkcję składową wskaźnikiem na składową klasy: 

```c++  
struct Para
{
    int nr = 0;
    std::string nazwa = "";
};

int main()
{
    std::vector<Para> pairs = {{2, "foo"}, {1, "bar"}, {0, "baz"}};
    std::ranges::sort(pairs, std::ranges::less{}, &Para::nr);
    // dalszy kod
}
```

Formalnie, wskaźnik na składową wygląda dokładnie tak, jak wskaźnik na funkcję składową: składa się ze znaku ampersand (`&`), nazwy klasy (tu: `Para`), operatora zakresu (`::`) i nazwy składowej (tu: `nr`). W tym przypadku jest to `&Para::nr`.

Oczywiście użyteczność projekcji nie ogranicza się tylko do sortowania. Oto kolejne dwa przykłady:

```c++ 
int ile_jedynek = std::ranges::count(pairs, 1, &Para::nr);
std::string s = std::ranges::max_element(pairs, {}, &Para::nazwa)->nazwa;
```

Pierwsza z powyższych instrukcji zlicza, ile razy w zakresie zdefiniowanym przez `pairs` składowa `nr` ma wartość `1`. Z kolei druga instrukcja w obiekcie `s` zapisuje wartość największej składowej `nazwa` (w sensie leksykograficznym) obiektów klasy `Para` znajdujących się w zakresie `pairs`.

Użyte w drugiej instrukcji wyrażenie `{}` oznacza "wartość domyślną w domyślnym type". W bibliotece *ranges* tą domyślną wartością jest obiekt funkcyjny `std::ranges::less`, który w większości praktycznych sytuacji odpowiada operatorowi `<`, który pozwala ustalić, który element zakresu jest "największy".

#### Podsumowanie

Projekcje nie są elementem biblioteki *ranges*, bez którego nie można się obejść. W końcu, w przypadku sortowania, moglibyśmy zdefiniować prostą lambdę o dwóch argumentach, `lhs` i `rhs`, w której kodzie występowałaby jedna, prosta instrukcja, `return proj(lhs) < proj(rhs));`, osiągając w ten sposób identyczny efekt, co z projekcjami. Niemniej, projekcje upraszczają zapis. 