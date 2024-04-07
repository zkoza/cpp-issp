## Listy inicjalizacyjne (preambuły) konstruktorów

Ten rozdział w kontekście dynamicznej tablicy (`Wektor`) ma charakter poboczny, niemniej, jak już pisać kod, to porządnie. 

Jak już wiemy, obiekty mają (czy raczej zwykle miewają)  konstruktory. Jednocześnie obiekty mogą być składowymi innych obiektów. Jak zainicjalizować składowe wymagające inicjalizacji? Jak zainicjalizować składowe z atrybutem `const`, który wyklucza możliwość przyrostowego konstruowania obiektu? Odpowiedź zwie się "lista inicjalizacyjna konstruktora". Inna nazwa tej listy to "preambuła konstruktora".

Rozpatrzmy przykład z poprzedniego rozdziału:

```c++    
class Wektor
{
    int* dane;
    size_t size;
    
  public:
    Wektor()
    {
        dane = nullptr;
        size = 0;
    }
};
```

Klasa `Wektor` ma dwie składowe: `dane` i `size`. Jak przekazać do konstruktora, który - poza dziwną deklaracją - jest zwyczajną funkcją, parametry inicjalizacyjne tych składowych? Oto odpowiednia składnia:

```c++ 
class Wektor
{
    int* dane;
    size_t size;
    
  public:
    Wektor()
    : dane(nullptr), size(0) 
    { }    
};
```

Jak widzimy, parametry inicjalizacyjne przekazuje się przed klamrą otwierającą treść funkcji, po dwukropku. Tworzą one listę danych i odpowiadających im parametrów inicjalizujących odpowiednie konstruktory. Nie rozumiesz? Gdy przyjdą bardziej zaawansowane programy, sprawa stanie się jasna. W tym miejscu ważne jest tylko to, by rozumieć, o co chodzi z tym dwukropkiem w definicjach konstruktorów. 

Dzięki liście inicjalizacyjnej mamy gwarancję, że w chwili, gdy rozpoczyna się wykonywanie konstruktora, wszystkie jego składowe mają już jakieś rozsądne wartości i można używać ich bezpiecznie. Nie napisałem, że są to wartości poprawne czy oczekiwane. Wystarczy, że są poprawne, czyli że gwarantują, że wszystkich składowych obiektu można w konstruktorze używać bezpiecznie. 

Po drobnych przekształceniach kod wygląda nastepująco:

```c++
#include <iostream>

class Wektor
{
    int* dane;
    size_t size;

  public:
    Wektor() 
        : dane(nullptr), size(0) 
        {}

    Wektor(size_t new_size, int init_value = 0) 
        : size(new_size)
    {
        dane = new int[new_size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = init_value;
        }
    }

    Wektor(const Wektor& v) 
        : size(v.size)
    {
        dane = new int[size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = v.dane[i];
        }
    }

    ~Wektor()
    {
        delete[] dane;
        dane = nullptr;
        size = 0;
    }
};

int main()
{
    Wektor w(5); // czy to się kompiluje?
    Wektor v(w); // czy to się kompiluje?
}
```

