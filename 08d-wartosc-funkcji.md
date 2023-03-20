### Wartość funkcji

Funkcja może, choć nie musi zwracać wartości. Jeżeli funkcja nie zwraca wartości, oznaczamy to w kodzie za pomocą słowa kluczowego `void` umieszczonego w miejscu typu wartości zwracanej przez funkcję:

```c++
#include <iostream>

void print_as_hex(int n)
{
    std::cout << std::hex << n << std::dec << "\n";
}
int main()
{
    print_as_hex(10 + 16*11 + 16*16*11 + 16*16*16*10);
}
```

Wynik działania tego programu, to liczba `10 + 16*11 + 16*16*11 + 16*16*16*10` w układzie szesnastkowym:

```txt
abba
```

Funkcja może zwracać wartość na takie same sposoby, w jakie może je przyjmować, a więc m.in. przez wartość, wskaźnik i referencję.  

Przykład definicji oraz użycia funkcji zwracającej wynik przez wartość:

```c++
#include <iostream>

int suma(int n, int m)
{
    return n + m;
}

int main()
{
    std::cout << suma(2, 3) << "\n"; // wyświetli 5   
}
```

Przykład definicji oraz użycia funkcji zwracającej wynik przez referencję:

```c++
#include <iostream>

std::ostream & dziwne()
{
    return std::cout;
}

int main()
{
    dziwne() << "Dziwny jest ten program\n";
}
```

 Wynik działania tego programu:

```txt
Dziwny jest ten program
```

W programie tym funkcja `dziwne` zwraca referencję do `std::cout`.  Dlatego instrukcja w funkcji `main` jest równoważna instrukcji

```c++
std::cout << "Dziwny jest ten program\n";
```

Funkcje zwracające wartość przez referencję są używane głównie przy przeciążaniu operatorów oraz przy definiowaniu funkcji składowych (w programowaniu obiektowym). 