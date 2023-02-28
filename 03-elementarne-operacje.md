## Operacje elementarne na typach wbudowanych

Skoro już wiemy, jak wewnętrznie reprezentowane są (czy też: mogą być) typy elementarne, pora na na jakąś akcję. Zacznijmy od tabeli skopiowanej z portalu [CppReference](https://en.cppreference.com/w/cpp/language/operator_arithmetic): 

![https://en.cppreference.com/w/cpp/language/operator_arithmetic](./img/03/common-operators.png)



Najczęściej używane podzielono w niej na 7 grup: operatory przypisania, inkrementacji i dekrementacji, arytmetyczne, logiczne, porównawcze, dostępu do składowej i inne. Omówię je w nieco innej kolejności niż przedstawia je powyższa tabela. 

#### 1. Operatory arytmetyczne

##### Promocje i konwersje standardowe 

Zgodnie z powyższą tabelą, w C++ wyróżnia się 13 operatorów, w tym 3 jednoargumentowe i 10 dwuargumentowych. Podstawowa zasada mówi, że jeżeli którykolwiek argument jest typu całkowitoliczbowego (np. `char` lub `bool`) i jego reprezentacja jest krótsza niż reprezentacja typu `int`, to ten argument jest ***promowany*** do typu `int`. Dlatego instrukcja (zwróć uwagę na `opertor +`)

```c++
std::cout << +'a' <<  "\n"; // Nie używaj tej sztuczki: preferuj zapis jawny, np. int('a')
```

nie wyświetla znaku `'a'`, lecz odpowiadającą mu liczbę typu `int` (tu: kod ASCII znaku `a`, czyli 97). Dla typu `bool` wartość `false` promowana jest do wartości 0, a `true` do wartości 1 (oczywiście w typie `int`). 

W przypadku operatorów dwuargumentowych dokonywane jest uzgodnieni ich typów tak, by typ "mniej ważny" został zamieniony na typ "bardziej ważny" bez zmiany wartości danego argumentu (o ile jest to możliwe), przy czym dla "krótkich" typów całkowitych najpierw wykonywana jest ich promocja do typu `int`. "Ważność" typów przedstawia się mniej więcej następująco: `long double` > `double`> `float`> `long long` > `long` > `int` > `short` > `signed char` > `bool`.   Jeżeli któryś z argumentów jest bez znaku (czyli posiada modyfikator `unsigned`), a drugi ma znak, to "zwycięża" typ argumentu o dłuższej reprezentacji, a jeżeli ich długości są sobie równe, to zwycięża typ bez znaku. Jeżeli oba są typami całkowitymi bez znaku o różnej długości reprezentacji bitowej, to "zwycięża" ten, który ma więcej bitów. Po uzgodnieniu typów obu argumentów wyznaczany jest wynik operacji, który ma taki sam typ, jak uzgodnione argumenty. 

Przykłady:

 ``` 
 auto i = '1' + 1   // i jest typu int i ma wartość 49, czyli wartość kodu ASCII litery '1' plus 1
 auto x = 1 + 1.0   // x jest typu double i ma wartość 2.0
 auto u = 1 - 2u    // u jest typu unsigned int i ma wartość 4294967295 (o ile unsigned reprezentowany jest na 32 bitach)
 auto z = 2 - true; // z jest typu int i ma wartość 2 - 1 = 1. Nie lubię tej konwersji. 
 auto q = -true;    // To poprawne wyrażenie o wartośći -1 w typie int, ale nie pisz tak
 double z = 1/2;    // z ma wartość 0, bo typem wyniku opertora dzielenia w wyrażeniu 1/2 jest int, a nie double
 ```

Dlaczego to jest takie skomplikowane? Bo każdy z typów C++ ma dobre zastosowania, choć w w większości bardzo specjalistyczne. Typy te mają bezpośrednie odpowiedniki w instrukcjach i możliwościach większości procesorów i pozwalają na bardzo dokładną kontrolę sposobu wykorzystania pamięci operacyjnej. Istnieją dużo łatwiejsze w użyciu języki, np, Python, w którym istnieją tylko 4 podstawowe typy arytmetyczne: `bool`, `int`, `float` i  `complex`, które podlegają prostej, wręcz naturalnej regule konwersji: `complex` > `float` > `int` >`bool`. Nie ma tu aż 3 typów zmiennopozycyjnych, liczb całkowitych nie dzieli się na "ze znakiem" i "bez znaku", w każdej z tych grup nie ma 5  różnych typów.  Ale też w przeciwieństwie do C++, (czysty) Python nie jest używany do pisania kompilatorów czy tworzenia zaawansowanego oprogramowania inżynierskiego działającego na bardzo dużych zbiorach danych czy na nietypowym sprzęcie.

#### Operatory jednoargumentowe `+` i `-`    

- Operator `+` nie robi nic (poza ewentualną promocją całkowitą), a operator `-` zwraca liczbę przeciwną do danej. Nigdy nie użyłem pierwszego z nich. 

```C++
int n = 1;
std::cout << +n << " " << -n << "\n"; // wyświetla 1 -1
```
#### Cztery podstawowe operacje arytmetyczne, czyli  +, -, *, /
operatory `+`, `-`, `*`, `/` mają z grubsza takie znaczenie, jak w szkole: reprezentują dodawanie, odejmowanie, mnożenie i dzielenie liczb. Jest jednak jeden haczyk: one działają inaczej, jeżeli ich argumentami są liczby całkowite, a inaczej, gdy zmiennopozycyjne. Z tego punktu widzenia w C++ mamy po dwa operatory  `+`, `-`, `*`, `/`  w zależności od tego, czy działają na liczbach całkowitych czy zmiennopozycyjnych. Odpowiadają im inne instrukcje assemblera, mogą być wykonywane w zupełnie innych rejestrach procesora. 

-  Dla argumentów całkowitych:
   -  Wynik dzielenia jest liczbą całkowitą - z wyniku dokładnego "odrzuca się" część ułamkową. Dlatego np. `5/4` ma wartość `1`, a `3/-2` to `-1`.    
   -  Jeżeli podczas dzielenia w mianowniku (czyli jako dzielnik) pojawi się zero, to mamy *[undefined behavior](https://en.cppreference.com/w/cpp/language/ub)*, program zwykle pada.  
   -  Jeżeli dokładna wartość wyniku dodawania, odejmowania lub mnożenia nie mieści się w zmiennej o docelowym typie, to mamy do czynienia z tzw. ***przepełnieniem*** (*overflow*). Dla typów bez znaku wynik uzyskuje się w [arytmetyce modulo $2^B$](https://pl.wikipedia.org/wiki/Arytmetyka_modularna), gdzie B  to liczba bitów, w jakich reprezentowany jest ten typ.  Dla typów ze znakiem (np. `int`) język nie określa sposobu wyznaczania wyniku - jest to kolejny przykład tzw. *undefined behavior*.  
-  Dla argumentów zmiennopozycyjnych:
   -  Jeżeli wartość wyniku przekracza zakres typu, w którym miałaby być zapisana, to tę wartość oznacza się jako `INF` (lub `-INF`). 
   -  Jeżeli wartości części ułamkowej wyniku nie można zapisać w mantysie dla danego typu, to wynik się zaokrągla
   -  Wynikiem niektórych operacji może być tzw. NaN (*Not a Number*), np. `0.0/0.0`, `INF - INF`, `sqrt(-1.0)`. 

#### Operator %

Operator `%` wyznacza resztę z dzielenia. Oba argumenty muszą być całkowite. Jeżeli w wyrażeniu `n % m` jeden z argumentów jest ujemny, to znak wyniku jest taki, jak znak pierwszego argumentu (czyli np.`4 % 3 ` = 1, `4 % -3` = 1, `-4 % 3 ` = -1 i  `-4 % -3` = -1).   

#### Operatory bitowe: ~, &, |, ^, << i >>

Operatory bitowe: `~`, `&`, `|`, `^`, `<<`, `>>` działają nie na liczbach, tylko na poszczególnych bitach. 

-  Operator ~ neguje ("odwraca") bity swojego argumentu. 

   ```C++
   int n = 2;   // n = 0b00000000000000000000000000000010
   int m = ~n;  // m = 0b11111111111111111111111111111101 = -3
   ```

-  Operator `&` wyznacza iloczyn bitowy swoich argumentów (*bitwise and*) - bit wyniku = 1, jeżeli odpowiadające mu bity argumentów są równe 1

   ```C++
   int n = 14;    // n = 0b00000000000000000000000000001110
   int m = 11;    // m = 0b00000000000000000000000000001011
   int k = n & m; // k = 0b00000000000000000000000000001010 = 12
   ```

-  Operator `|` wyznacza sumę bitową swoich argumentów (*bitwise or*) - bit wyniku = 1, jeżeli choć jeden z odpowiadających mu bitów argumentów jest  równy 1

   ```C++
   int n = 14;    // n = 0b00000000000000000000000000001110
   int m = 11;    // m = 0b00000000000000000000000000001011
   int k = n | m; // k = 0b00000000000000000000000000001111 = 15
   ```

-  Operator `^` wyznacza różnicę symetryczną swoich argumentów (*bitwise xor*) - bit wyniku = 1, jeżeli odpowiadające mu bity argumentów mają różne wartości (tj. `0^0` = 0, `1^1` = 0, `0^1` = 1, `1^0` = 1)

   ```C++
   int n = 14;    // n = 0b00000000000000000000000000001110
   int m = 11;    // m = 0b00000000000000000000000000001011
   int k = n ^ m; // k = 0b00000000000000000000000000000101 = 5
   ```

-  Operator `<<` przesuwa bity w lewo: ` a << b ` to `a` z bitami przesuniętymi w lewo o `b` pozycji.  `b`  nie może być ujemne. Bity, które się nie mieszczą w typie wyniku, są odrzucane.  Zasadniczo operacja ta równoważna jest mnożeniu przez $2^b$, nawet jeśli `a` jest ujemne (od C++20).   

   ```C++
   int n = 14;     // n = 0b00000000000000000000000000001110
   int m = 2;      // m = 0b00000000000000000000000000000010
   int k = n << m; // k = 0b00000000000000000000000000111000 = 4*n = 56
   ```

-  Operator `>>` przesuwa bity w prawo: ` a >> b ` to `a` z bitami przesuniętymi w prawo o `b` pozycji.  `b`  nie może być ujemne. Bity, które się nie mieszczą w typie wyniku, są odrzucane.  Operacja ta równoważna jest dzieleniu przez $2^b$ (z odrzuceniem części ułamkowej), nawet jeśli `a` jest ujemne (od C++20). 

   ```C++
   int n = 14;     // n = 0b00000000000000000000000000001110
   int m = 2;      // m = 0b00000000000000000000000000000010
   int k = n >> m; // k = 0b00000000000000000000000000000011 = n/4 = 3
   ```

Operatory bitowe umożliwiają operacje na pojedynczych bitach. Na przykład poniższa funkcja sprawdza, czy wartość `k`-tego bitu liczby `n` jest ustawiona na 1:

```C++
bool is_set(unsigned n, unsigned k)
{
    const unsigned mask = 1u << k;
    return mask & n;  // równoważne: "return (mask & n) != 0;", bo zwracana wartość jest konwertowana na typ bool 
}
```



### Operatory przypisania

Działanie najprostszego operatora jest dość proste: `a = b` przypisuje wartość zmiennej `b` do zmiennej `a`. Jeżeli typy tych zmiennych są różne, nastepuje konwersja typu `b` do typu `a`. Może to spowodować utratę dokładności. Na przykład:

```C++
int a = 3.14 // a = 3
```

Uwaga! Operatory przypisania mają wartość i mogą pojawić się w wyrażeniach arytmetycznych. Legalny jest więc zapis z = `a + (b = c)`, ale nikt przytomny tego nie wykorzystuje (a w każdym razie - nie w takim kontekście).  Wartością operatora `=` jest jego lewy argument po zakończeniu operacji przypisania. Ponieważ operator przypisania ma wartość, legalna i czasami spotykane jest incjalizowanie wielu zmiennych w jednej instrukcji (nie zalecam):

```C++
a = b = c = 0; // równoważne ciągowi instrukcji:  c = 0; b = c; a = b; 
```

Operatory przypisania są [prawostronnie łączne](https://pl.wikipedia.org/wiki/Operator_(programowanie)#%C5%81%C4%85czno%C5%9B%C4%87,_wi%C4%85zanie), więc kompilator zinterpretuje powyższą instrukcję tak, jak by programista wstawił nawiasy od prawej do lewej:

```C++
a = (b = (c = 0)); // równoważne ciągowi instrukcji:  c = 0; b = c; a = b; 
```

 Słynny, legalny przykład zastosowania operatora przypisania w argumencie pętli `while` do kopiowania napisów w tradycyjnym stylu języka C:

  ```C++
  while (*p++ = *q++) continue;
  ```

Nie zaleca się nadużywania tego, że operatory przypisania mają wartość i rzadko kiedy można się z tym spotkać w tzw. kodzie produkcyjnym.  Piszę o tym głównie dlatego, że jest to źródłem nieskończonej liczby pomyłek osób uczących się C++, które mylą operator przypisania `=`  z  operatorem porównania `==`. Typowy przykład takiej pomyłki:

```C++
if (a = 1) // chyba miało być "if (a == 1)"? 
{
    // jakiś kod
}
```

W powyższym przykładzie kompilator przypisze 1 do `a`, następnie skonwertuje typ `a` do `bool`, co w tym przypadku daje wartość `true`, a więc uzna, że warunek jest zawsze spełniony. 

Powyższy przykład ilustruje zjawisko ***efektu ubocznego***. Zasadniczo instrukcja `if` tylko sprawdza spełnienie jakiegoś warunku, tu zaś mamy dodatkowe, raczej nieoczekiwane w tym kontekście zdarzenie: nadanie zmiennej `a` wartości 1. Jak ognia unikajmy efektów ubocznych.     

Pozostałe operatory sprowadzają się do operatora przypisania w następującego schematu:

- `a += b` jest równoważne wyrażeniu `a = a + b`;
- `a *= b` jest równoważne wyrażeniu `a = a * b`;

 ...

- `a >>= b` jest równoważne wyrażeniu `a = a >> b`;

 Prawda że to jest proste? 

```C++
int n = 3; 
int m = 5;
m -= n;  // m = m - n     = 2;
n >>= 1; // n = n >> 1    = 3 >> 1    = 1
```

### Inkrementacja i dekrementacja

Operator inkrementacji (zwiększenia), `++`, zwiększa wartość swojego argumentu o 1, a operator dekrementacji (zmniejszenia) zmniejsza tę wartość o 1. Niespodzianką jest zwykle to, że operator ten można zapisywać zarówno prze, jak i po zmiennej, na której działa i dokonanie tego wyboru może istotnie zmienić sens wyrażenia. Różnica między `i++` i `++i` dotyczy wartości tych wyrażeń: wartością `i++` jest "stara wartość `i` *czy ta sprzed zwiększenia o 1), a wartością `++i` jest wartość nowa (zwiększona). Czyli zapis

```C++
int x = ++i;
```

 jest równoważny

```C++
++i;
int x = i;
```

Natomiast instrukcja

```C++
int x = i++;
```

Jest równoważna ciągowi instrukcji

```C++
int x = i;
i++;
```

Operatory te wprowadzono dawno, dawno temu, gdy kompilatory dość słabo optymalizowały tworzony przez siebie kod. I tak, powyższy ciąg instrukcji mógł być interpretowany następująco:

- wczytaj wartość `i` z pamięci RAM do rejestru r
- zapisz wartość rejestru r w zmiennej `x` w pamięci RAM
- wczytaj `i` do rejestru r
- zwiększ `r` o jeden
- zapisz wartość rejestru `r` w `i`   

 Tu jest mnóstwo niepotrzebnej pracy (dwukrotne wczytywanie `i`). Zapis `int x = i++` mógł być interpretowany w bardziej efektywny sposób:

- wczytaj `i` do rejestru r
- zapisz wartość rejestru r w zmiennej `x`
- zwiększ wartość rejestru r o 1 
- zapisz wartość rejestru r w zmiennej `i`

Jedna (kosztowna!) instrukcja mniej. Podobne powody stały za wprowadzeniem złożonych operatorów przypisania, np. `+=`. Dziś to już jednak prehistoria. Kompilator doskonale poradzi sobie z niemal dowolnym zapisem kodu, on "widzi" znacznie większe fragmenty kodu niż pojedyncze funkcje, apekt optymalizacyjny nie ma już żadnego znaczenia.

Z operatorem `++` (i `---`) wiąże się kilka problemów, które prowadzą do nieszczęsnego *undefined behavior*.  Na przykład niech `f` będzie funkcją dwuargumentową, i rozpatrzmy 

```C++
int x = 0;
f(x++, x++);
```

W wyrażeniu tym nie wiemy, w jakiej kolejności opracowywane będą argument funkcji i kiedy dokładnie nastąpi inkrementacja zmiennej `x`. Być może funkcję wywołamy jako `f(0,1)`, może jako `f(1,0)`, a może nawet jako `f(0,0)`. Inny przykład to wyrażenie `x++ - ++x`; Tu mamy ten sam problem: może to jest `0 - 2`, może `1 - 1`, a może jeszcze coś innego. Dlatego 

- Unikaj operatorów `++` i `--` w złożonych wyrażeniach
- Nigdy w jednym wyrażeniu nie stosuj wielokrotnie tych operatorów do tej samej zmiennej.
- Program zawsze można napisać tak, by te operatory były jedynymi operatorami w wyrażeniu, czyli 

```C++
x++; // lub równoważnie: ++x;
--y; // lub równoważnie: y--;
```

Operatory `++` i `--` można stosować do liczb zmiennopozycyjnych, ale w praktyce się tego unika. 

Wszytko powyższe stosuje się tak samo do operatora `--` z wyjątkiem tego, że on wartość swojego argumentu zmniejsza a nie zwiększa o 1.

```C++
// Ta pętla wyświetli 5 4 3 2 1 0 
for(int i = 5; i >= 0; --i)
    std::cout << i << " ";     
```

### Operatory logiczne

 

 

