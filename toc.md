# Programowanie w C++ dla ISSP

## Spis treści

### Preliminaria

- [Wstęp](./00-wstep.md)
- [Narzędzia](./01-narzedzia.md)
  - [bitbucket, github, gitlab](./01a-githubs.md) 
  - [git](./01b-git.md)

### Nieobiektowe jądro języka

- [Typy wbudowane](./02-typy-wbudowane.md)

- [Elementarne operacje na typach wbudowanych](./03-elementarne-operacje.md)

- [Arytmetyka liczb zmiennopozycyjnych](./04-arytmetyka-zmiennopozycyjna.md)

- [Tablice](05-tablice.md)
- [Wskaźniki](./06-wskazniki.md)
  - [`qsort` w akcji](06a-qsort.md)

- Instrukcje złożone
  - instrukcja warunkowa (if)
  - pętla for
  - pętla while
  - pętla do ... while
  - instrukcja switch

- [Struktury](./07-struktury.md)
- [Referencje](./08-referencje.md)
- [Funkcje](./08-funkcje.md)
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


### Klasy i obiekty

- Przykład: własna implementacja dynamicznego wektora
  - [Jako struktura](./09a-dyn-vec-struct.md) ("w stylu języka C")
  - Jako klasa
    1. [*First things first*: funkcje składowe i sekcje *public*, *private* (i *protected*)](./09a-dyn-vec-member-fun-public-private.md) 
    2. [Dodajemy konstruktor i destruktor](./09b-dyn-vec-konstruktor-destruktor.md)
    3. [Do konstruktorów dodajemy listy inicjalizacyjne](./09c-dyn-vec-konstruktor-preambula.md) (opcja)
    4. [Dodajemy operator indeksowania](./09d-dyn-vec-operator-indeksowania.md)  
    5. [Dodajemy składowe funkcyjne z atrybutem const](./09e-dyn-vec-skladowe-const.md) 
    6. [Dodajemy operator przypisania](./09f-dyn-vec-assignment-operator.md) (używając pseudowskaźnika `this` )
    7. Rozdzielamy implementację na interfejs i implementację
    8. Dodajemy  możliwość zmiany rozmiaru wektora
    9. Dodajemy konstruktor z listą inicjalizacyjną (opcja)
    10. Dodajemy iterator (opcja)

- Klasy
  - konstruktory, destruktor
  - hermetyzacja danych
  - składowe statyczne
  - składowe typy danych
- Dziedziczenie
- [Funkcje wirtualne](./14-funkcje.wirtualne.md)
  - [Statyczne i dynamiczne łączenie funkcji](14a-laczenie-funkcji.md)
  - [Typowa implementacja wirtualnego wywołania funkcji w C++](14b-wywolanie-wirtualne.md)
  - [Przykłady](14c-przyklady-wirtualne.md) 

### Strumienie

- Obsługa plików
- Obsługa strumieni skojarzonych z pamięcią RAM
- std::format


### Preprocesor

- Do czego służy preprocesor?
- Podstawowe dyrektywy preprocesora
- Makra
- Kompilacja warunkowa
- Preprocesor a inne elementy języka

### Szablony

- Szablony funkcji
- Szablony klas

### Obsługa błędów

- Wyjątki
- Asercje
- Debuger

