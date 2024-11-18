# Problem optymalizacyjny w programowaniu dynamicznym

Jak wspomniałem wcześniej, programowanie dynamiczne wiąże się z występowaniem trzech elementów:

1. Problem można rozwiązać za pomocą rekurencji
2. Problem wymaga zastosowania techniki zwanej spamiętywaniem (ang. *memoisation*).
3. Problem dotyczy konstrukcji optymalnego algorytmu znajdującego rozwiązanie jakiegoś zadania optmalizacyjnego (np. znalezienia najkrótszej drogi dla listonosza mającego dostarczyć przesyłkę do *N* odbiorców).

W poprzednik rozdziale przedstawiłem, w jaki sposób można w C++ rozwiązywać problemy zawierające punkty 1 i 2 z powyższej listy. Teraz czas na pełny zestaw 3 właściwości charakteryzujących algorytmy wykorzystujące programowanie dynamiczne.  

Rozwiążmy następujący problem:

*Szewczyk Dratewka chce u czarnoksiężnika kupić księżniczce Karolinie $n$ klejnotów. Czarnoksiężnik stawia warunek: klejnoty ustawione są w szeregu, a każdemu z nich przypisana jest liczba naturalna $a_i \ge 1$. Dratewka może po kolei wskazywać klejnoty, z wyjątkiem dwóch skrajnych, i wtedy jego cena równa będzie iloczynowi przypisanej mu liczby oraz liczb przypisanych dwóm aktualnie sąsiednim klejnotom. Ostatnie dwa klejnoty Dratewka otrzyma za darmo. Pomóż Dratewce znaleźć optymalną strategię kupowania klejnotów!*

Zilustrujmy powyższy problem na prostym przykładzie. Załóżmy, że Dratewka chce kupić 4 klejnoty, a odpowiadający im ciąg $\{a_i\} = \{1, 10, 2, 10\}$. Jeżeli najpierw wybierzemy klejnot nr 2, dla którego $c_2 = 10$, to jego cena wyniesie $1 \times 10 \times 2 = 20$. Na stole pozostanie ciąg $\{1, 2, 10\}$. Musimy wybrać klejnot środkowy, którego cena wynosi   $1 \times 2 \times 10 = 20$. Jeżeli jednak klejnoty kupowalibyśmy w odwrotnej kolejności, najpierw wybierając klejnot trzeci z $c_3 = 2$, to zapłacimy za niego $10 \times 2 \times 10 = 200$ i na stole zostanie ciąg  $\{1, 10, 10\}$, którego cena wynosić będzie  $1 \times 10 \times 10 = 200$. W zależności od kolejności wyboru klejnotów, albo ich łączna cena wyniesie 40, albo 400. Dratewka ma więc nad czym się zastanawiać!

Zastanówmy się najpierw, jaki może być koszt wyznaczenia optymalnej strategii metodą siłową (*brute force*), czyli testując na rympał wszelkie możliwe kolejności kupna klejnotów. Jest ich oczywiście tyle, ile permutacji zbioru o $n-2$ elementach, czyli $(n-2)!$ Już dla $n$ rzędu 30 na wynik czekalibyśmy dłużej niż istnieje Wszechświat! 

Jak uprościć problem za pomocą rekurencji, czyli przez sprowadzenie problemu większego do nieco mniejszego lu, ogólnie, do skończonej liczby problemów mniejszych? Rozwiązanie wymaga pewnej pomysłowości, jednak nie jest trudne. Otóż spośród kamieni $c_2, c_3, \ldots, c_{n-1}$ w optymalnej strategii któryś zostanie wybrany jako ostatni. Jeżeli tak, to optymalne wybory kamieni na lewo od niego i tych na prawo od niego stają się problemami rozłącznymi, niezależącymi od siebie. W rzeczy samej, jeżeli indeks tego ostatniego kamienia oznaczymy literą $k$, to iloczyny trzech kolejnych $c_i$, które wchodzą w skład ostatecznej ceny, nigdy nie będą zawierały innych wariantów zawierającą $c_i$ z lewej strony i $c_j$ z prawej, niż $c_1 \times c_k \times c_n,$ czyli ceny pobrania ostatniego kamienia (pamiętamy, że $c_1$ jest zawsze skrajnym lewym, a $c_n$ - skrajnie prawym kamieniem). Dlaczego? Jeżeli w iloczynie $c_i \times c_j \times c_l$ indeks `k` występuje na lewej pozycji, to $j$ oraz $l$ muszą należeć do kamieni leżących na prawo od $k$, a więc w podciągu $c_{k+1}\ldots c_n$. Podobnie jeżeli $k$ występuje na prawej pozycji, to  $j$ oraz $l$ muszą należeć do kamieni leżących na lewo od $k$, a więc w podciągu $c_{1}\ldots c_{k-1}$. Jeżeli $k$ jest na pozycji środkowej, to jest wyjmowany, ale z definicji $k$ jest wyjmowany na końcu, więc w środku występuje tylko w iloczynie $c_1 \times c_k \times c_n$. Oznacza to, że można problem rozbić na pytanie o optymalną strategię dla podciągu $\{c_1, c_2,\ldots,c_k\}$, w którym $k$ może wystąpić w iloczynach  $c_i \times c_j \times c_l$ na ostatnim miejscu i drugie, o optymalną strategię dla $\{c_k, c_{k+1},\ldots,c_n\}$, dla którego iloczyny  $c_i \times c_j \times c_l$  mogą zawierać $k$ na lewym miejscu. W ten sposób problem $n$-elementowy sprowadziliśmy do problemów mniejszych, $k$-elementowego oraz  $n-k+1$-elementowego. Pamiętamy bowiem, że $2 \le k \le n-1$, co utwierdza nas w przekonaniu, że te problemy naprawdę są mniejsze. 

Jeżeli powyższy podział względem "elementu ostatniego" poprowadzimy dalej, to zobaczymy, że wystarczy spamiętać optymalne rozwiązania dla wszystkich ciągów $\{a_i, a_{i+1}, \ldots, a_{j-1}, a_j\}$ gdzie $1\le i < j \le n$.  Jest ich mniej więcej $n^2/2$, a więc znacznie, znacznie mniej niż $(n-2)!$. Na przykład jeżeli dla ciągu 9-elementowego $a_1, a_2, a_3, a_4, a_5, a_6, a_7, a_8, a_9$ ostatnim kamieniem jest ten z indeksem 6, to problem rozbija się na 2 problemy optymalizacji ciągów $a_1, a_2, a_3, a_4, a_5, a_6$ oraz $a_6, a_7, a_8, a_9$. Jeżeli w tym pierwszym, 6-elementowym podciągu ostatnim kamieniem jest ten z indeksem 3, to sprowadziliśmy problem 6-elementowy do kolejnych dwóch problemów, tym razem 3-elementowego $a_1, a_2, a_3$ oraz 4-elementowego  $a_3, a_4, a_5, a_6$. Zawsze, na każdym poziomie rekurencyjnej redukcji problemu, mamy do czynienia z ciągami $\{a_i, a_{i+1}, \ldots, a_{j-1}, a_j\}$ będącymi podciągiem kolejnych klejnotów w ich oryginalnym ciągu. 

Takie podciągi kolejnych elementów ciągu można sparametryzować zaledwie dwoma liczbami naturalnymi - parą indeksów ich pierwszego i ostatniego elementu. Dzięki temu oryginalny problem o złożoności obliczeniowej rzędu $O(n!)$ można zredukować do problemu co najwyżej kwadratowego, czyli $O(n^2)$. 

Rozwiązanie przedstawię w odwrotnej kolejności niż to, zaprezentowane w poprzednim rozdziale dla klasy `Newton`, czyli metodą *top-down*, od ogółu do szczegółu.  

#### Program testowy

Program testowy dla naszego rozwiązania może wyglądać bardzo prosto, np. tak:

```c++
#include "klejnoty.h"
#include <iostream>

int main()
{
    std::vector a = {4, 2, 300, 1, 2, 2, 300};
    Klejnoty klejnoty(a);
    std::cout << klejnoty.optimal_price() << "\n";
}
```

Definiujemy ciąg "cen" poszczególnych klejnotów, $a_i$. Następnie inicjalizujemy nim obiekt klasy `Klejnoty`. Optymalną cenę uzyskujemy jego funkcją składową `optimal_price`. Proste!

#### Plik nagłówkowy

Plik nagłówkowy klasy `Klejnoty` może wyglądać następująco:

```c++   
#ifndef KLEJNOTY_H
#define KLEJNOTY_H

#include <vector>

class Klejnoty
{
  public:
    Klejnoty(std::vector<int> c)
        : c{c}
    {
        init_tab();
        _optimal_price = best_price(0, c.size() - 1);
    }
    int optimal_price() const { return _optimal_price; }

  private:
    std::vector<int> c;
    std::vector<std::vector<int>> tab;
    int _optimal_price = -1;

    int best_price(int first, int last);
    void init_tab();
};

#endif
```

Mamy tu konstruktor przyjmujący na wejściu "ceny" poszczególnych klejnotów. Zapisuje on je w swojej składowej prywatnej `c`. Trzeba przy tym pamiętać, że od tej pory posługujemy się ciągiem $c_i$ indeksowanym od zera a nie jedynki. Konstruktor funkcją `init_tab` inicjalizuje prywatną tablicę `tab` (ustala jej rozmiary oraz wartości początkowe elementów), a następnie funkcją `best_price`  wypełnia jej elementy `[i][j]` najlepszą ceną dla podciągu $\{a_i, a_{i+1}, \ldots, a_{j-1}, a_j\}$ . Zapisuje jednocześnie najlepszą cenę dla całego ciągu $c_i$ w zmiennej `_optimal_price`. 

W konstrukcji klasy `Klejnoty` dostrzec można podstawową cechę rozwiązań rekurencyjnych ze spamiętywaniem.  Mamy tu bowiem obiekt zawierający jakąś użyteczną funkcję rekurencyjną (tu: prywatną `best_price`) oraz kontener spamiętujący rozwiązania częściowe dla mniejszych problemów (tu: `tab`) .

#### Plik z implementacją klasy

Plik `klejnoty.cpp` może wyglądać nastepująco:

```cpp
#include "klejnoty.h"

#include <cassert>
#include <limits>

void Klejnoty::init_tab()
{
    const int invalid_value = -1;
    tab.resize(c.size());
    for (int i = 0; i < tab.size(); i++)
    {
        tab[i].resize(c.size(), invalid_value);
    }
}

int Klejnoty::best_price(int first, int last)
{
    assert(first >= 0);
    assert(last >= 0);
    assert(first < tab.size());
    assert(last < tab[first].size());
    assert(last >= first);

    if (tab[first][last] >= 0)
        return tab[first][last];
    if (last - first <= 1)
    {
        tab[first][last] = 0;
        return tab[first][last];
    }

    int current_best = std::numeric_limits<int>::max();
    for (int i = first + 1; i <= last - 1; i++)
    {
        auto left = best_price(first, i);
        auto right = best_price(i, last);
        auto middle = c[first] * c[i] * c[last];
        current_best = std::min(current_best, left + right + middle);
    }
    tab[first][last] = current_best;
    return tab[first][last];
}
```

Funkcja `Klejnoty::init_tab()` ustala bezpieczny rozmiar dwuwymiarowej tablicy `tab` używanej do spamietywania wyników. Nie jest to rozwiązanie optymalne, gdyż zamiast tablicy trójkątnej wystarczyłaby tu tablica trójkątna. 

Funkcja `int Klejnoty::best_price(int first, int last)` to serce całej klasy. To jest właśnie ta funkcja rekurencyjna, która wykorzystuje spamiętywanie wyników dla mniejszych problemów, by wygenerować rozwiązanie dla problemu głównego (`n`-elementowego). Seria asercji na jej początku to mój prosty sposób na wyszukiwanie prostych błędów bez uciekania się do profesjonalnych testów jednostkowych. Nie mają one znaczenia dla logiki tej funkcji. Następnie w dwóch instrukcjach warunkowych `if` funkcja ta obsługuje dwa przypadki, w których można wyznaczyć jej wartość bez uciekania się do rekurencji. Jednym z nich jest sytuacja, gdy wartość funkcji mamy już zapisaną w tablicy `tab` w poprzednich wywołaniach funkcji `best_price`. Drugim są ciągi co najwyżej dwuelementowe, dla których cena pobrania klejnotu równa się zero. 

Jeżeli żaden z tych warunków nie jest spełniony, przystępujemy do rozwiązania rekurencyjnego. Ponieważ optymalizujemy cenę w dół, więc pierwotnie jej wartość początkową ustalamy na największą liczbę reprezentowalną w typie `int`.  Następnie w pętli sprawdzamy koszt pobrania każdego kamienia, od drugiego do przedostatniego (z definicji nie możemy pobrać kamieni skrajnych), wybierając wariant, dla którego całkowity koszt pobrania kamieni od `first` do `last` jest najmniejszy. Na koniec zapisujemy uzyskany przed chwilą wynik w tablicy (spamiętywanie, *memoizacja*) i zwracamy go jako wartość funkcji.

Na koniec ciekawostka.  

### Problemy

1. Przedstawiony powyżej problem "Szewczyka Dratewki" jest równoważny pewnemu znanemu problemowi z programowania dynamicznego, mającemu w chwili pisania tych słów swoją stronę w Wikipedii w 10 językach. O jaki znany problem tu chodzi? 
2. Udowodnij, że jeżeli Czarnoksiężnik przedstawił Dratewce ciąg niemalejący $a_1, a_2, \ldots a_n$, gdzie $1 \le a_1 \le a_2 \ldots  \le  a_n$, to optymalna strategia polega na kupowaniu klejnotów od lewej do prawej, czyli, kolejno, $a_2$, potem $a_3$, $a_4$ i tak aż do $a_{n-1}$. 
   * czy podobną właściwość posiadają też ciągi nierosnące?
   * czy można tę właściwość zastosować do istotnego zmniejszenia średniej liczby wywołań rekurencyjnych funkcji `best_price`?

3. Liczby Catalana zdefiniowane są zależnością rekurencyjną

   $\displaystyle C_0 = 1 \quad \text{oraz} \quad C_{n}=\sum_{i=1}^{n}C_{i-1}C_{n-i}\quad\text{for }n > 0$
   Napisz klasę `Catalan` ze składową rekurencyjną `operator(int n)` wykorzystującą spamiętywanie (*memoization*) do wyznaczenia *n*-tej liczby Catalana, $n \ge 0$.  

4. Napisz program wyznaczający liczbę różnych drzew BST z `n` węzłami o wartościach $1, 2,\ldots,n$ . W tym celu napisz równanie rekurencyjne sprowadzające problem $n$-węzłowy do problemów o mniejszej liczbie węzłów, a następnie zaimplementuj je (klasa, rekurencyjna funkcja składowa, spamiętywanie). 

5. Napisz program (klasa, rekurencyjna funkcja składowa, spamiętywanie) wyznaczający liczbę różnych dróg, którymi z węzła (0, 0) można dojść do węzła $(n, n)$, gdzie $n$ jest liczbą całkowitą $\ge0$, poruszając się wyłącznie w prawo lub do góry o jedną jednostkę i nigdy nie przekraczając diagonali $y = x$. Por. rysunek dla $n=4$, z którego wynika, że $C_4 = 14$:
   ![](/home/zkoza/Pulpit/Dydaktyka/aaa_ProgCPP/github/cpp-issp/advanced/img/z02/grid_example.png)

6. Szewczyk Dratewka został zaproszony do skarbca. Są w nim, oczywiście, skarby. Każdy ma swoją wagę $w_i$ i cenę $c_i$, przy czym zarówno wagi, jak i ceny są wyrażone w liczbach naturalnych. Za zabicie każdego smoka Dratewka może ze skarbca wziąć spośród N skarbów tyle, ile chce, byle ich waga nie przekraczała $W$. Napisz program, który pomoże Dratewce wybrać skarby o możliwie największej sumarycznej cenie dla danych $N$, $W$, $\{w_i\}$ oraz $\{c_i\}$, gdzie $i = 1,2,\ldots,N$.

