### Co to są dynamiczne struktury danych?

Na początek krótkie przypomnienie. W programowaniu często można się spotkać z określeniami ***statyczny*** i ***dynamiczny***. Na przykład mówi się, że istnieją języki programowania ze statyczną kontrolą typów oraz z dynamicznym systemem typów, podobnie istnieją funkcje łączone statycznie i funkcje łączone dynamicznie. Generalnie, zasada jest prosta:

- jeżeli coś jest ***statyczne***, to znaczy, że jest obsługiwane już podczas kompilacji (ang. ***at compile-time***). W praktyce, jeżeli coś jest statyczne, to jest ustalane przed rozpoczęciem działania programu i nie może  może ulec zmianie podczas jego działania.   
- jeżeli coś jest ***dynamiczne***, to znaczy, że jest obsługiwane dopiero podczas działania programu, czyli, jak mawiają Anglosasi, ***at runtime***. W praktyce jeżeli coś jest dynamiczne, to w ogólnym przypadku nie można przewidzieć jego wartości (lub innych cech) aż do wywołania się programu.  

I tak, statyczna kontrola typów w C++ oznacza, że kompilator musi znać typ każdej zmiennej i obiektu przed pierwszym jej użyciem i typ ten nie może ulec zmianie podczas działania programu. Dzięki temu kompilator może przejąć kontrolę nad zarządzaniem pamięcią przydzieloną takim obiektom, a program nie musi co chwilę sprawdzać poprawności argumentów przekazywanych do funkcji, dzięki czemu działa efektywnie. Z kolei dynamiczna kontrola typów odbywa się dopiero podczas wykonywania się programu. Dzięki temu w wielu językach skryptowych, jak Python czy Matlab, nie musimy specyfikować typów argumentów funkcji. Na marginesie: do języków ze statycznym systemem typów należą m.in.: Ada, C, C++, C#, Fortran, Go, Java, Kotlin, Rust, Swift, TypeScript, z kolei do języków z dynamicznym systemem typów należą m.in.:  JavaScript, Lisp, Logo, Julia, Matlab, Objective-C, Perl, PHP, Prolog, Python, R, Ruby i Smalltalk. 

Z powyższego wynika, że statyczna struktura danych ma ustaloną swoją strukturę już podczas kompilacji, a dynamiczna może zmieniać swoją strukturę (reprezentację komputerową) podczas działania programu.

Przykładem ***statycznej struktury danych*** w C++ jest po prostu zwykła zmienna, np. 

```c++
int n = 9;
```

Po powyższej deklaracji wiadomo, że `n` zawsze będzie typu `int`. Bardziej złożonymi przykładam statycznych struktur danych w C++ są tablice i struktury:

```c++   
int tab[9];
struct Circle
{
   Color color;
   double radius;
   Point center;
};
Circle c;
```

W przeciwieństwie do języków jak JavaScript, w strukturze `Circle` nie możemy w C++ dodawać nowych pól czy usuwać już tych zdefiniowanych. W tej chwili chyba staje się w końcu jasne, dlaczego w C++ rozmiar tablicy musi być stałą kompilacji, tzn. jej rozmiar musi być znany kompilatorowi. 

Typowymi dla C++ przykładami **dynamicznych struktur danych** są **kontenery** udostępnione w **bibliotece standardowej**. Choć występuje w niej jeden wyjątek: `std::array` jest kontenerem statycznym, jego rozmiar jest zawsze stały i ustalany podczas kompilacji. Stąd prosta wskazówka: dynamiczne struktury danych zazwyczaj reprezentują kontenery o zmiennej pojemności, tj. o składowych umożliwiających dodawanie lub usuwanie danych. Jeżeli w jakimś kontenerze C++ występuje choć jedna składowa o nazwie `insert`, `push_back`,  `pop_back`, `push_front`, `pop_fron`, `resize`, `erase`, `clear`, to niemal na pewno dany kontener zaimplementowano jako dynamiczną strukturę danych. Należą do nich m.in.:

- `std::vector` (dynamiczna tablica o regulowanej podczas działania programu liczbie elementów)
- `std::set` (drzewo czerwono-czarne przechowująca wyłącznie klucze)
- `std::map` (drzewo czerwono-czarne przechowująca pary klucz-wartość)
- `std::unordered_set` (tablica mieszająca przechowująca wyłącznie klucze)
- `std::unordered_map` (tablica mieszająca przechowująca pary klucz-wartość)
- `std::forward_list` (lista pojedynczo wiązana)
- `std::list` (lista podwójnie wiązana)
- `std::priority_queue` (sterta, zwykle zaimplementowana na bazie tablicy dynamicznej)

Dla porządku przedstawiam też (zapewne niepełną) listę **statycznych struktur danych** będących **kontenerami** **biblioteki standardowej** C++:

-  `std::array` (tablica o określonej, niezmiennej długości)
- `std::flat_set` (C++23, jak `std::set`, ale oparty na tablicy dynamicznej a nie drzewie czerwono-czarnym)
- `std::flat_map` (C++23, jak `std::map`, ale oparty na tablicy dynamicznej a nie drzewie czerwono-czarnym)

Powyższe zestawienie nie jest pełne!

------



#### Literatura:

- https://stackoverflow.com/questions/408670/stack-static-and-heap-in-c

- https://en.cppreference.com/w/cpp/container 

  
