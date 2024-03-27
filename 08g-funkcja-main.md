### Funkcja `main`

#### Argumenty funkcji `main`

Wykonanie każdego programu w języku C++ rozpoczyna się od inicjalizacji obiektów globalnych  (np. `std::cout`), a następnie sterowanie przekazywane jest do funkcji `main`. Funkcja ta występuje w dwóch wariantach: jako funkcja bezargumentowa lub dwuargumentowa. Wersja bezargumentowa jest prosta: 

```c++
int main()
{
    std::cout << "Ala ma kota\n";
}
```

Wersja dwuargumentowa używana jest, gdy chcemy przekazać do programu argumenty wiersza poleceń:  

```c++
#include <iostream>

int main(int argc, const char* argv[])
{
    std::cout << "argc = " << argc << "\n";
    for (int i = 0; i < argc; i++)
        std::cout << "argument " << i << ": " << argv[i] << "\n";
}
```

Jeżeli wywołamy ten program tak:

```bash
./a.out Ala ma 2 koty "w cudzysłowie"
```

to program może wyświetlić co następuje:

```bash
argc = 6
argument 0: ./a.out
argument 1: Ala
argument 2: ma
argument 3: 2
argument 4: koty
argument 5: w cudzysłowie
```

Jak widzimy, wartość `argc`  odpowiada liczbie argumentów wiersza poleceń powiększoną o 1. Z kolei `argv` to (w wystarczającym przybliżeniu) tablica napisów w stylu języka C przechowujących kolejne argumenty wiersza poleceń, przy czym `argv[0]`, przechowuje nazwę programu (zwykle wraz ze ścieżką). Nazwy obu argumentów funkcji `main`, czyli `argc` i `argv` są zwyczajowe i oczywiście można zamiast nich użyć innych identyfikatorów, ale po co wprowadzać zamieszanie?  

#### Wartość funkcji `main` 

Funkcja `main` zwraca liczbę całkowitą jako sygnał dla systemu operacyjnego o statusie swojego zakończenia. Jeżeli pominiemy w niej instrukcję `return`, to kompilator wstawi za nas na jej końcu instrukcję  

```c++
return EXIT_SUCCESS; // return 0;
```

oznaczającą prawidłowe zakończenie programu. Wartość stałej `EXIT_SUCCESS` wynosi 0. Każda inna wartość z reguły interpretowana jest jako kod błędu. Jeżeli uruchomimy program zwracający wartość `1` 

```c++
int main()
{
  return 1;
}
```

to możemy ją przechwycić w konsoli. Np. w powłoce `bash` wartość ta przechowywana jest w zmiennej `$?`:

```bash
> ./a.out
> echo $?
1
echo $?
0 
```

Pierwsza komenda `echo $?` wyświetliła liczbę zwróconą z funkcji `main`programu `./a.out`, czyli 1, natomiast druga - kod zakończenia instrukcji `echo $?`, czyli 0. Zero oznacza bowiem "polecenie zakończyło się bez błędu". 

Wartości przekazywane z funkcji `main` można badać w skryptach uruchamiających rożne programy. Dzięki temu skrypty mogą np. przerwać swoje działanie, gdy okaże się, że któryś z programów zakończył się niepowodzeniem, i wyświetlać komunikaty diagnostyczne opisujące napotkany problem. 