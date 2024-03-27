## Stos wywołań funkcji (*call stack*)

Nie mam wątpliwości, że programista programujący w językach kompilowanych do kodu maszynowego (C, C++, Rust i wiele, wiele innych) powinien mieć choćby elementarną wiedzę o stosie wywołań funkcji i pamięci wolnej. Ponieważ moja wiedza w tym zakresie jest właśnie taka elementarna, ograniczę się tu zasadniczo do przedstawienia linku do artykułu w Wikipedii:

- https://en.wikipedia.org/wiki/Call_stack 

To, o czym powinien pamiętać programista C++ można, z grubsza, zapisać w następujących punktach:

- Stos funkcji jest współcześnie typowym mechanizmem organizacji pamięci stosowanym podczas  wywoływań funkcji
- Nazwane zmienne i obiekty lokalne w funkcji, jej parametry i zwracana przez nie wartość zapisywane są na stosie wywołań funkcji
- Stos wywołań funkcji ma z góry ograniczony rozmiar (w Linuksie typowo 8 MB na wątek)
- Po zakończeniu wywołania funkcji pamięć zajmowana przez funkcję na stosie jest automatycznie zwalniana
- Obiekty umieszczone na stosie funkcji mogą zarządzać pamięcią zarezerwowaną w pamięci wolnej
- Pamięcią zarezerwowaną na stosie funkcji zarządza kompilator, pamięcią zarezerwowaną w pamięci wolnej - programista
- Dobrze napisane programy automatyzują zarządzanie pamięcią wolną - por. wzorzec programistyczny RAII. Dzięki temu "programista końcowy", czyli np. użytkownik bibliotek, nie musi ręcznie zarządzać pamięcią wolną 
- Kontener `std::array` rezerwuje pamięć na stosie, natomiast `std::vector` i pozostałe kontenery biblioteki standardowej C++ przechowują dane w pamięci wolnej
- Do niskopoziomowego, ręcznego zarządzania pamięcią wolną służą operatory `new`, `new[]`, `delete` i `delete[]`.
- Stan stosu wywołań funkcji można badać debuggerem

Do najczęstszych błędów związanych z niezrozumieniem organizacji pamięci w programach napisanych w C++ należą:

- przepełnienie stosu (*stack overflow*), które występuje, gdy jakaś funkcja wywoła się rekurencyjnie zbyt dużą liczbę razy lub gdy będziemy rezerwować na stosie zbyt duże tablice
- wyciek pamięci, gdy nasz program "zapomni" zwolnić nieużywaną pamięć zarezerwowaną w pamięci wolnej

 Przykład funkcji (`next`), która generuje *stack overflow*:

```c++
#include <iostream>

int next(int n)
{
   std::cout << "n = " << n << std::endl;
   return next(n + 1);
}

int main()
{
  next(1);
}
```

  Program ten wyświetla

```txt
n = 1
...
n = 261821
n = 261822
Naruszenie ochrony pamięci (zrzut pamięci)
```

skąd można wywnioskować, że każde wywołanie funkcji `next`zajmuje na stosie funkcji 32 bajty (8192 KB / 261823 = 32,04...).

Jeżeli w funkcji `next` zadeklarujemy jakąś dużą zmienną lokalną, np tablicę 500 tysięcy int-ów

```c++
int next(int n)
{
   int tab[500'000] = {0};
	 std::cout << "n = " << n << std::endl;
   return next(n + 1);
}
```

To program "padnie" już przy jej piątym wywołaniu:

```txt
> ./a.out
n = 1
n = 2
n = 3
n = 4
Naruszenie ochrony pamięci (zrzut pamięci)
```

Podczas sesji z debuggerem może to wyglądać tak:

```txt
n = 1
n = 2
n = 3
n = 4

Program received signal SIGSEGV, Segmentation fault.
0x00005555555551a4 in next (n=<error reading variable: Cannot access memory at address 0x7fffff67461c>) at prog.cpp:4
4	{
```