## Składowe funkcyjne z atrybutem `const`

Spróbujmy implementację, którą do tej chwili mamy, zastosować w bardziej złożonych warunkach. Na przykład w funkcji przyjmującej `Wektor` przez stałą referencję (czyli `const&`):

```c++
void test(const Wektor& w)
{
    std::cout << w[0] << "\n";
}

int main
{
    Wektor w(5);
    test(w);
}
```

Kompilator zgłasza błąd: 

```txt
vector5.cpp: In function ‘void test(const Wektor&)’:
vector5.cpp:52:21: error: passing ‘const Wektor’ as ‘this’ argument discards qualifiers [-fpermissive]
   52 |     std::cout << w[0] << "\n";
      |                     ^
vector5.cpp:41:10: note:   in call to ‘int& Wektor::operator[](size_t)’
   41 |     int& operator[](size_t index) { return dane[index]; }
      |          ^~~~~~~~
```

Kluczowa informacja to komunikat *passing ‘const Wektor’ as ‘this’ argument discards qualifiers*. W praktyce oznacza, że w jakimś miejscu użyliśmy kwalifikatora (ang. qualifier) `const`, a w innym użyliśmy jakiegoś wyrażenia, które stoi z nim w sprzeczności. Co ciekawe, jeżeli w deklaracji funkcji `test` usuniemy kwalifikator `const`,  to program skompiluje się bez żadnych zastrzeżeń. Z komunikatu kompilatory wnioskujemy, że błędne użycie "czegoś z `const`" nastąpiło w instrukcji ` std::cout << w[0] << "\n";`  i że kompilator nie może pogodzić tego "czegoś" z  próbą wywołania funkcji  `int& Wektor::operator[](size_t)`. O co chodzi? 

Żeby odpowiedzieć na to pytanie, musimy wrócić do znaczenia `const` w deklaracji `void test(const Wektor& w)`.  Wiemy, że deklaracja ta mówi, że funkcja `test` przyjmuje swój argument przez stałą referencję, czyli że nie zmienia wartości obiektu, do którego ta referencja się odnosi. No dobrze, ale co to znaczy? Otóż ten `const` oznacza, że zobowiązujemy się nie używać w kodzie funkcji `test` żadnych instrukcji lub wywołań funkcji, które choćby potencjalnie mogłyby zmieniać stan obiektu `w` i żądamy, by każdą próbę złamania tej obietnicy kompilator traktował jak błąd. Skąd kompilator ma wiedzieć, które funkcje potencjalnie mogą, a które nie mogą modyfikować swojego argumentu? Oczywiście nie zagląda do ich kodu - programy w C++ opierają się na bibliotekach i niezależnej kompilacji różnych jednostek kompilacji, przy czym kompilator ma dostęp do kodu źródłowego tylko jednej z nich. Oznacza to, że kompilator decyzje podejmuje na podstawie deklaracji. Jeżeli jakaś funkcja przyjmuje jakiś argument przez stałą referencję, to kompilator zaakceptuje wywołanie w jej treści innych funkcji przyjmujących argumenty przez wartość lub stałą referencję, bo wie, że takie funkcje nie deklarują zamiaru modyfikacji swoich argumentów. 

No dobrze. Jak więc rozwiązać problem tego `const`  w naszej klasie `Wektor`? Rozwiązanie jest proste, choć nieco nieoczekiwane: C++ umożliwia deklarowanie funkcji składowych z modyfikatorem `const`. Oto przykład:

```c++   
class Wektor
{
    ...
  public:  
	int& operator[](size_t index) { return dane[index]; }       // działa na nie-const
    int operator[](size_t index) const { return dane[index]; }  // działa na const
};
```

Okazuje się, że można definiować dwie funkcje o identycznych nazwach i identycznych argumentach, różniących się jednie modyfikatorem `const` zapisanym między listą argumentów a treścią funkcji! 

Jak to działa? Jak pamiętamy, funkcje składowe wywołuje się zawsze na obiektach, stosując "notację z kropką", przy czym obiekt, na którym ta funkcja jest wywoływana, zapisywany jest przed kropką. Rozpatrzmy ogólny przykład obiektu `obj` i wywoływanej na nim funkcji `foo()`:

```c++ 
obiekt.foo(); // wywołanie funkcji składowej
```

Kompilator sprawdza *deklaracje* obiektu i funkcji w poszukiwaniu modyfikatora `const` (np. jak w deklaracji argumentu powyższej funkcji test: `const Wektor& w`). 

-  Jeżeli w danej klasie zdefiniowano **jedną funkcję** `foo` **nieposiadającą modyfikatora** `const`, to można wywołać ją tylko na takim obiekcie tej klasy, który sam nie ma modyfikatora `const`. Innymi słowy: brak `const` w definicji funkcji składowej oznacza dla kompilatora (i doświadczonego programisty), że intencją twórcy tej funkcji była możliwość modyfikowania za jej pośrednictwem obiektu `obj`. 
-  Jeżeli w danej klasie zdefiniowano **jedną funkcję** `foo` **posiadającą modyfikator** `const`, to można wywołać ją na każdym obiekcie tej klasy, niezależnie od tego, czy ma on w swojej deklaracji modyfikator `const` czy nie. Innymi słowy:  użycie `const` w definicji funkcji składowej oznacza dla kompilatora (i doświadczonego programisty), że intencją twórcy tej funkcji jest zablokowanie możliwości modyfikowania za jej pośrednictwem obiektu `obj`. Jeśli tak, to taką funkcję można wywołać na każdym obiekcie.
-  Jeżeli w danej klasie zdefiniowano **dwie funkcje** `foo`, jedną **posiadającą modyfikator** `const` a drugą jej nieposiadającą, to
  - funkcję z modyfikatorem `const` kompilator będzie wywoływał wyłącznie na obiektach, które same mają modyfikator `const`, 
  - a tę bez modyfikatora `const` będzie wywoływał wyłącznie na obiektach pozbawionych tego modyfikatora.

Funkcje składowe z modyfikatorem `const` są więc bardziej uniwersalne od tych bez niego. Dlatego używaj go zawsze, o ile oczywiście ma to sens, czyli gdy dana funkcja składowa nie powinna modyfikować obiektu, na którym będzie wywoływana.    

### Przykłady:

1. Klasa z dwoma operatorami indeksowania, pierwszy bez modyfikatora `const`, drugi z `const`. W tym przypadku zawsze wywoła się "jakiś" `operator[]`, przy czym to, który, zależy od tego, czy użyto modyfikatora `const` w deklaracji obiektu, na którym ten operator działa. Przykład:

   ```c++
   class Wektor
   {
    ...
     public:      
   int& operator[](size_t index) { return dane[index]; }       // działa na nie-const
    int operator[](size_t index) const { return dane[index]; }  // działa na const
   };
   
   void test(const Wektor& w)
   {
       std::cout << w[0] << "\n"; // w zadeklarowano z const => wywoła się drugi operator[]
   }
   
   int main
   {
       Wektor w(5);
       test(w);
       return w[0]; // w zadeklarowano bez const => wywoła się pierwszy operator[]   
   }
   ```

2. Klasa z jednym operatorem indeksowania, z modyfikatorem `const`. W tym przypadku powyższy  program testowy dalej będzie działał (tyle że nie używamy w nim tego operatora do modyfikacji obiektu)

   ```c++
   class Wektor
   {
       ...
     public:      
       int operator[](size_t index) const { return dane[index]; }  // działa wszędzie
   };
   
   void test(const Wektor& w)
   {
       std::cout << w[0] << "\n"; // OK, wywoła operator[]  
   }
   
   int main
   {
       Wektor w(5);
       test(w);
       // w[0] = 1; to by nie przeszło, bo stały operator[] nie może modyfikować w 
       return w[0]; // OK, wywoła operator[]
   }  
   ```

3. Klasa z jednym operatorem indeksowania, bez modyfikatora `const`. W tym przypadku powyższy  program testowy nie będzie działał: takiego operatora nie można wywoływać na obiektach stałych:

   ```c++
   class Wektor
   {
       ...
     public:      
   	int& operator[](size_t index) { return dane[index]; } // działa tylko na nie-const
   };
   
   void test(const Wektor& w)
   {
       std::cout << w[0] << "\n"; // Błąd!, w jest const, a operator[] nie jest!  
   }
   
   int main
   {
       Wektor w(5);
       test(w);
       w[0] = 1;    // Ok 
       return w[0]; // OK, wywoła operator[]
   }
   ```

## Inne proste funkcje składowe (z trybutem `const` i bez) 

Teraz w naszej klasie możemy łatwo zaimplementować kolejne funkcje składowe, które można też odnaleźć w implementacjach klasy `std::vector`.  Przy okazji zmieniłem nazwy składowych z danymi z `size` na `_size` i z `dane` na `_dane`, gdyż bardzo chcę mieć funkcję o nazwie `size`. Gdyby moje dane nie byłyby chronione w części prywatnej i gdybym był twórcą biblioteki publicznej, to nigdy bym tego nie mógł zrobić, bo użytkownicy tej biblioteki z pewnością w swoich programach używaliby składowych o nazwach `size` i `dane`.  Kod nowych funkcji nie wymaga wielkiego komentarza. Funkcje bez `const` zwracają dane, zapisane w obiekcie, przez referencję, dlatego można ich używać do modyfikacji obiektu (np. `w.front() = 1;`). Ani `front`, ani `back` nie sprawdzają, czy wektor nie jest pusty - użycie ich na pustym wektorze może spowodować pad programu. 

```c++
int& front() { return _dane[0]; }           // referencja do pierwszego elementu wektora
int front() const { return _dane[0]; }      // wartość pierwszego elementu wektora
int& back() { return _dane[_size-1]; }      // referencja do ostatniego elementu wektora
int back() const { return _dane[_size-1]; } // wartość ostatniego elementu wektora  
size_t size() const { return _size; }       // rozmiar wektora
int* data () const {return _dane; }         // wskaźnik do lokalizacji danych
bool empty() const { return _size == 0; }   // czy wektor jest pusty?
```

Nasz program może teraz wyglądać tak:

```c++
// proste składowe const i nie-const

#include <iostream>
#include <stdexcept>

class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;

  public:
    Wektor() {}

    Wektor(size_t new_size, int init_value = 0)
        : _size(new_size)
    {
        _dane = new int[_size];
        for (size_t i = 0; i < _size; i++)
        {
            _dane[i] = init_value;
        }
    }

    Wektor(const Wektor& v)
        : _size(v._size) 
    {
        _dane = new int[_size];
        for (size_t i = 0; i < _size; i++)
        {
            _dane[i] = v._dane[i];
        }
    }

    ~Wektor()
    {
        delete[] _dane;
        _dane = nullptr;
        _size = 0;
    }

    int& operator[](size_t index) { return _dane[index]; }
    int operator[](size_t index) const { return _dane[index]; }
    int& at(size_t index)
    {
        if (index >= _size) 
            throw std::out_of_range("Index out of range in Wektor::at(size_t)");
        return _dane[index];
    }
    int at(size_t index) const
    {
        if (index >= _size) 
            throw std::out_of_range("Index out of range in Wektor::at(size_t)");
        return _dane[index];
    }
    int& front() { return _dane[0]; }      // referencja do pierwszego elementu wektora
    int front() const { return _dane[0]; } // wartość pierwszego elementu wektora
    int& back() { return _dane[_size-1]; } // referencja do ostatniego elementu wektora
    int back() const { return _dane[_size-1]; } // wartość ostatniego elementu wektora
    size_t size() const { return _size; }       // rozmiar wektora
    int* data () const {return _dane; }         // wskaźnik do lokalizacji danych
    bool empty() const { return _size == 0; }   // czy wektor jest pusty?
};

void test(const Wektor& w)
{
    std::cout << w[0] << "\n";
    std::cout << w.size() << "\n";
    std::cout << w.empty() << "\n";
}

int main()
{
    Wektor w(5);
    test(w);
    w[1] = 8;
    w.front() = 1;
    w.back() = -1;
}
```

### Wnioski i zalecenia

W miarę możliwości używaj stałych funkcji składowych. Po pierwsze, w ten sposób pomożesz kompilatorowi zapewnić poprawność twojego programu, po drugie, w ten sposób twój program będzie się sam komentował: użycie lub nieużycie modyfikatora `const` w funkcjach składowych to wyraźny sygnał co do intencji programisty. Pamiętaj też, że nawyk stosowania stałych funkcji składowych jest jednym z prostych i pewnych sposobów identyfikacji poziomu zaawansowania w C++: ich brak w czyimś kodzie jest jak podpis programisty niedzielnego.
