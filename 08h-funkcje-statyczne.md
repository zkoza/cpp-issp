### Funkcje statyczne
To obecnie rzadko spotykany rodzaj funkcji, odziedziczony z języka C, w C++ zastępowany umieszczeniem definicji funkcji w tzw. nienazwanej przestrzeni nazw. Temat nieco zaawansowany, związany z projektami i dzieleniem dużych programów na małe pliki źródłowe.  Funkcje statyczne są "niewidoczne" dla konsolidatora, co oznacza, że w różnych plikach źródłowych można mieć funkcje o tej samej nazwie i typach argumentów i nie będzie to uznane za pogwałcenie reguły ODR. Innym słowy: funkcja statyczna to funkcja "prywatna" dla danego pliku źródłowego (*.cpp). Ponieważ funkcje statyczne używane są w C++ rzadko, nie podaję nawet pełnego przykładu ich użycia. Przykład uproszczony:

```c++
static int suma(int x, int y)
{
    return x + y;
}

int main()
{
   std::cout << suma(5, 6) << "\n";
}
```

Modyfikator `static` oznacza tu, że tej konkretnej funkcji `suma` nie można użyć w innym pliku niż ten, w którym znajduje się jej definicja. Te inne pliki mogą mieć swoje funkcje swobodne `suma`. Ma to pewne znaczenie w dużych projektach.

Więcej na temat statycznych funkcji swobodnych można znaleźć w Internecie, szukając informacji na temat "funkcje statyczne w języku C" lub "static functions in C". Nie w C++, tylko w C. W przypadku C++ wyszukiwarki zwrócą raczej informacje o statycznych funkcjach składowych, a to zupełnie inna para kaloszy.   