# Funkcje

W informatyce "funkcja" to stały zestaw instrukcji wykonujących jakieś konkretne zadanie i zorganizowany w funkcjonalną jednostkę. Funkcje często grupuje się w biblioteki i - jak z klocków Lego -  można z nich tworzyć programy.  Spotyka się też nazwy "podprogram", "procedura" i "metoda" (w j. angielskim: *function*, *method*, *pocedure*, *routine*, *subprogram*, *subroutine*, *callable unit*). 

[Wikipedia](https://en.wikipedia.org/wiki/Function_(computer_programming)) wylicza następujące zalety stosowania funkcji:

- [Dekompozycja](https://en.wikipedia.org/wiki/Decomposition_(computer_science)) złożonego zadania programistycznego na zadania prostsze. Wraz ze [strukturami danych](https://en.wikipedia.org/wiki/Data_structure) jest to podstawowe narzędzie [programowania strukturalnego](https://en. wikipedia.org/wiki/Structured_programming)
- Ograniczenie zjawiska [duplikowania kodu](https://en.wikipedia.org/wiki/Duplicate_code) w programie.
- Umożliwienie [ponownego wykorzystania kodu](https://en.wikipedia.org/wiki/Code_reuse) w wielu programach.
- Ułatwienie pracy zespołowej nad dużym zadaniem programistycznym i dzielenia go na etapy.
- [Ukrycie szczegółów implementacji](https://en.wikipedia.org/wiki/Information_hiding) przed użytkownikami funkcji, co ułatwia aktualizację ich implementacji bez wpływania na kod ich użytkowników   
- Poprawa czytelności kodu poprzez zastąpienie bloku kodu wywołaniem funkcji, której nazwa opisuje zadanie wykonywane przez ten blok. Kod wywołujący funkcję staje się zwięzły i czytelny i z tego powodu zaleca się dzielić duże zadania na funkcje nawet wtedy, gdy tych funkcji nie wykorzystuje się nigdzie indziej. 
- Ułatwienie [śledzenia](https://en.wikipedia.org/wiki/Traceability#Software) wykonywania się kodu; bez podziału kodu programu na funkcje, jego debugowanie byłoby poważnie utrudnione

Funkcje w C++ dzielimy na dwa główne rodzaje: ***funkcje swobodne*** i ***funkcje składowe***. Funkcje swobodne to te, które wywoływane są wg składni 

```c++
nazwa_funkcji(arg_1, arg_2,..., arg_n)          // funkcja swobodna
```

natomiast funkcje składowe wywoływane są wg składni "z kropką": 

```c++
obiekt.nazwa_funkcji(arg_1, arg_2,..., arg_n)   // funkcja składowa
```

Funkcje składowe zostaną omówione przy okazji omawiania klas; poniżej przedstawiam wyłącznie funkcje swobodne

### Funkcje (swobodne) w C++

Temat funkcji jest rozległy, dlatego podzieliłem go na mniejsze części:

- [Definicja i deklaracja funkcji](./08a-deklaracja-i-definicja.md)

- [Reguła ODR](./08b-regula-ODR.md)

- [Przekazywanie argumentów do funkcji](./08c-argumenty-funkcji.md)

- [Wartość funkcji](./08d-wartosc-funkcji.md)

- [Argumenty domyślne funkcji](./08e-argumenty-domyslne.md)

- [Operatory jako funkcje](./08o-operatory.md)

- [Funkcje `inline`](./08f-funkcje-inline.md)

- [Funkcja `main`](./08g-funkcja-main.md)

- [Funkcje statyczne](./08h-funkcje-statyczne.md)

- [Stos wywołań funkcji (*call stack*)](./08i-call-stack.md)

  
