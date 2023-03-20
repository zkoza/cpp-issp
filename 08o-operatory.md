## Operatory jako funkcje

Jedna z fundamentalnych zasad, wg których zaprojektowano C++, głosi, że cokolwiek można zrobić z typem wbudowanym (np. `int`), można też zrobić z typem zdefiniowanym przez użytkownika, np. ze strukturami. Jeżeli więc poprawna jest instrukcja `std::cout << 5`, to w podobny sposób powinno się móc wyświetlić zawartość dowolnego obiektu. W tym celu potrzebna jest możliwość zdefiniowania znaczenia wyrażenia `std::cout << x` dla `x` dowolnego typu. W C++ istnieje specjalna składnia umożliwiająca definicję znaczenia operatorów dla argumentów dowolnych typów. Sprawę powinny wyjaśnić poniższe przykłady.

#### Przykład 1: operator<<

Załóżmy, że mamy strukturę `Pracownik`:

```c++
struct Pracownik
{
	std::string nazwisko;
    int data_urodzenia;
};
```

Aby kompilator mógł zinterpretować wyrażenie `std::cout << x` dla `x` typu `Pracownik`, możemy zdefiniować specjalną funkcję, `operator<<`:

```c++
std::ostream& operator<<(std::ostream& out, const Pracownik& prac)
{
    out << "{nazwisko: " << prac.nazwisko 
        << ", data_urodzenia: " << prac.data_urodzenia << "}";
    return out;
}
```

Możemy ten kod łatwo przetestować:

```c++
int main()
{
    Pracownik p {"Mick Jagger", 1943};
    std::cout << p << "\n";
}
```

 Wynik działania tego programu:

```txt
{nazwisko: Mick Jagger, data_urodzenia: 1943}
```

Jak widzimy, operatory definiujemy jak funkcje o nazwie `operator<<`, `operator+`, `operator*`, `operator!`, `operator&`, `operator&&` itd. Jeżeli operator jest dwuargumentowy, to wyrażenia `x << y` jest równoważne wyrażeniu `operator<<(x, y)`. Dlatego powyższą funkcje `main` można też zapisać w dość dziwnej postaci

```c++
int main()
{
    Pracownik p {"Mick Jagger", 1943};
    operator<<(operator<<(std::cout, p), "\n");
}
```

Ten program działa dokładnie tak samo, jak poprzedni. 

Widzimy więc, że aby wyrażenie `std::cout << prac` miało dla kompilatora znaczenie, pierwszy argument tego operatora musi być referencją do wyjściowego strumienia danych (gdyż `std::cout` jest jednym ze strumieni wyjścia), drugi zaś musi być typu `Pracownik`. Strumienie zawsze przekazujemy przez referencję, gdyż nie wolno ich kopiować, a poza tym chcemy coś zapisywać do strumienia, więc chcemy go modyfikować. Stąd deklaracja argumentów: `operator<<(std::ostream& out, const Pracownik& prac)`. 

Typ `std::ostream` jest uniwersalnym typem wspólnym dla wszystkich strumieni wyjścia, dlatego nasz operator będzie działał także na plikach:

```c++
int main()
{
 	std::ofstream out("plik.txt"); // otwiera plik "plik.txt" i wiąże go z obiektem out 
    Pracownik p {"Mick Jagger", 1943};
    out << p << "\n";              // zapisuje w pliku:
                                   //   {nazwisko: Mick Jagger, data_urodzenia: 1943}
}
```

Pozostaje wyjaśnić, dlaczego ten operator zwraca referencję do strumienia. Chodzi o to, że chcemy mieć możliwość używania go w całych ciągach operacji wyjścia, jak w wyrażeniu 

```c++
std::cout << p << "\n";
```

w którym wynik wyrażenia `out << p` jest lewym argumentem drugiego operatora `<<`. Jeżeli pierwszy operator zwróci referencję do `std::cout`, to powyższa instrukcja będzie równoważna następującym dwóm:

```c++
std::cout << p;
std::cout << "\n";
```

 #### Przykład 2: operator*

Oto sposób na to, by wektory zmiennych typu`int` można było mnożyć przez liczby całkowite:

```c++
std::vector<int>& operator*=(std::vector<int> & v, int k)
{
    for (auto & n: v) 
        n *= k;
    return v;    
}
```

Program testowy:

```c++
#include <iostream>
#include <vector>

std::vector<int>& operator*=(std::vector<int> & v, int k)
{
    for (auto & n: v) 
        n *= k;
    return v;        
}

int main()
{
    std::vector<int> v = {1, 2, 3};
    v *= -2;
    for (auto n: v)
        std::cout << n << " ";
    std::cout << "\n";
}
```

Wynik:

```txt
-2 -4 -6 
```

Analogicznie można zdefiniować niemal wszystkie operatory dwuragumentowe i większość jednoargumentowych. Niemal pełna lista: `+`, `-`, `*`, `/`, `%`, `^`, `&`, `|`, `~`, `!`, `=`, `<`, `>`, `+=`, `-=`, `*=`, `/=`, `%=`, `^=`, `&=`, `|=`, `<<`, `>>`, `>>=`, `<<=`, `==`, `!=`, `<=`, `>=`, `<=>`, `&&`, `||`, `++`, `--`, `,`, `->*`, `->`, `( )`, `[ ]`. W niektórych przypadkach trzeba doczytać dokumentację, by dowiedzieć się np. jak odróżnić `++x` od `x++`. Można też przeciążać m.in. operatory `new` i `delete` operatory konwersji typu oraz "operatory przyrostkowe".  Oto [przykład](https://en.cppreference.com/w/cpp/chrono/operator%22%22h) zastosowania standardowych operatorów przyrostkowych:

```c++
#include <iostream>
#include <chrono>
 
int main()
{
    using namespace std::chrono_literals;
    // day będzie typu std::chrono::hours
    auto day = 24h;        // h jest operatorem przyrostkowym
    auto halfday = day/2;
    std::cout << "one day is " << day.count() << " hours\n"
              << "half a day is " << halfday.count() << " hours\n";
}
```

Wynik:

```txt
one day is 24 hours
half a day is 12 hours
```
