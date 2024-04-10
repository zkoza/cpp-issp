##  Interfejs i implementacja klasy

Nasza klasa (`Wektor`) nieco się rozrosła i mimo, że ma niecałe 100 wierszy, powoli traci czytelność. Ponadto w tym samym pliku mamy zarówno definicję klasy, jak i (prosty) kod, który ma ją testować. Jedno i drugie jest dość niewygodne. Nie tak wyglądają "profesjonalne" klasy. Zwykle programy dzieli się na części, każdą w osobnym pliku. Kompilator kompiluje każdy z nich osobno, po czym łączy te fragmenty razem (wraz z gotowymi, skompilowanymi bibliotekami) w jeden plik wykonywalny. Podstawowy podział to podział na pliki źródłowe (zwykle z rozszerzeniem cpp) i nagłówkowe (zwykle z rozszerzeniem h), które często występują parami (plikom źródłowym, poza `main.cpp`, praktycznie zawsze towarzyszy plik nagłówkowy; pliki nagłówkowe mogą zaś występować bez plików źródłowych).  W przypadku klas, z reguły każda zapisywana jest w dwóch plikach, jednym źródłowym i jednym nagłówkowym. Plik źródłowy zawiera kod kompilowany przez kompilator, a plik nagłówkowy - interfejs klasy, który umożliwia sprawne i bezpieczne łączenie tego kodu z innymi częściami programu. Spójrzmy, jak to może wyglądać w przypadku klasy `Wektor`. 

### Interfejs

Czysty interfejs klasy nie zawiera kodu, a ogranicza się wyłącznie do deklaracji. Zapisywany jest w pliku nagłówkowym, np. `vector7.h`. W przypadku klasy `Wektor` jego zawartość może wyglądać następująco: 

```c++      
#ifndef VECTOR7_H
#define VECTOR7_H

#include <cstddef>    // tu jest definicja size_t

class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;

  public:
    Wektor();
    Wektor(size_t new_size, int init_value = 0);
    Wektor(const Wektor& v);
    ~Wektor();

    Wektor& operator=(const Wektor& rhs);
    int& operator[](size_t index);
    int operator[](size_t index) const;
    int& at(size_t index);
    int at(size_t index) const;
    int& front();
    int front() const;
    int& back();
    int back() const;
    size_t size() const;
    int* data() const;
    bool empty() const;
};

#endif  // VECTOR7_H
```

Mamy tu kilka nowości. Po pierwsze, treść plików nagłówkowych chroniona jest zwykle przez tzw. strażnika pliku nagłówkowego, który składa się z trzech makr preprocesora: `#indef`, `#define` i `#endif`:

```c++ 
#ifndef VECTOR_H
#define VECTOR_H

    Interfejs klasy 

#endif     
```

Makro `#ifndef` sprawdza, czy zdefiniowano pewne unikatowe w całym programie makro (tu: `VECTOR_H`), które zwykle zapisuje się dużymi literami (jeśli plik nazywa się `vector7.h`, to makro w strażniku pliku nagłówkowego zwyczajowo nazywa się `VECTOR7_H`). Konstrukcja strażnika gwarantuje, że w złożonych programach treść danego pliku nagłówkowego zostanie w każdej jednostce kompilacji włączona do pliku źródłowego co najwyżej jeden raz.

Druga nowość to brak treści funkcji (czyli klamer `{ }`  i kodu wewnątrz nich). Takie nagłówki funkcji są ich deklaracjami. Kompilator potrzebuje ich, by móc sensownie łączyć program podzielony na małe części. Nie potrzebuje do tego treści funkcji, a jedynie informacji o ich nazwie, liczbie i typie argumentów oraz typie wartości funkcji. Powyższa plik jest bardzo klarowny. Czytając tak napisany plik nagłówkowy, można łatwo i szybko zorientować się, w jaki sposób można używać obiektów danej klasy.

Kod źródłowy funkcji umieszcza się w plikach źródłowych, w naszym przypadku - w pliku `vector7.cpp`. Może on wyglądać następująco:

```c++      
#include "vector7.h"

#include <stdexcept>

Wektor::Wektor() {}

Wektor::Wektor(size_t new_size, int init_value)
    : _size(new_size)
{
    _dane = new int[_size];
    for (size_t i = 0; i < _size; i++)
    {
        _dane[i] = init_value;
    }
}

Wektor::Wektor(const Wektor& v)
    : _size(v._size)
{
    _dane = new int[_size];
    for (size_t i = 0; i < _size; i++)
    {
        _dane[i] = v._dane[i];
    }
}

Wektor::~Wektor()
{
    delete[] _dane;
    _dane = nullptr;
    _size = 0;
}

Wektor& Wektor::operator=(const Wektor& rhs)
{
    if (this == &rhs) return *this;

    if (size() != rhs.size())
    {
        delete[] _dane;
        _dane = new int[rhs.size()];
    }
    for (size_t i = 0; i < rhs.size(); i++)
    {
        _dane[i] = rhs[i];
    }
    _size = rhs.size();
    return *this;
}

int& Wektor::operator[](size_t index) { return _dane[index]; }

int Wektor::operator[](size_t index) const { return _dane[index]; }

int& Wektor::at(size_t index)
{
    if (index >= _size) 
        throw std::out_of_range("Index out of range in Wektor::at(size_t)");
    return _dane[index];
}

int Wektor::at(size_t index) const
{
    if (index >= _size) 
        throw std::out_of_range("Index out of range in Wektor::at(size_t)");
    return _dane[index];
}

int& Wektor::front() { return _dane[0]; }

int Wektor::front() const { return _dane[0]; }

int& Wektor::back() { return _dane[_size - 1]; }

int Wektor::back() const { return _dane[_size - 1]; }

size_t Wektor::size() const { return _size; }

int* Wektor::data() const { return _dane; }

bool Wektor::empty() const { return _size == 0; }

```

Plik źródłowy niemal zawsze rozpoczyna się od włączenia odpowiadającego mu pliku nagłówkowego zawierającego deklarację klasy:

```c++ 
#include "vector7.h"
```

Dzięki temu "wie", że identyfikator `Wektor` to nazwa klasy i jakie funkcje składowe ta klasa powinna definiować.  

Uważne oko dostrzeże w powyższym kodzie pewną zmianę w stosunku do sytuacji, gdy kod funkcji składowych znajdował się wewnątrz klasy `Wektor`, teraz bowiem nazwy wszystkich funkcji składowych klasy `Wektor` poprzedzone są jej nazwą i operatorem zakresu, `::`. Czyli np. zamiast `empty`, definiuje się funkcję  `Wektor::empty`. Wynika to stąd, że wewnątrz definicji klasy, czyli wewnątrz klamer obejmujących jej definicję: 

```c++ 
class Wektor
{
   ...  
};
```

kompilator wszystkie składowe umieszcza w przestrzeni nazw danej klasy (tu: `Wektor`), dlatego pełne nazwy  definiowanych tam składowych czy funkcji, np. `Wektor::size`, można skracać do samej nazwy funkcji. Jeżeli to wyjaśnienie wciąż jest zbyt trudne, to wystarczy zapamiętać, że pełne nazwy funkcji składowych klas zawierają nazwy przestrzeni nazw i że te przestrzenie nazw możemy pomijać w kodzie, jeżeli tych funkcji używamy wewnątrz klas lub jeśli wywołujemy je na obiektach danej klasy. Innymi słowy, obie poniższe instrukcje są poprawne i równoważne:

```c++ 
std::cout << w.empty() << "\n";
std::cout << w.Wektor::empty() << "\n";  // poprawne, ale przekombinowane
```

ale zwykle stosuje się zapis skrócony, czyli bez przestrzeni nazw funkcji składowej. Zapis z nazwą przestrzeni nazw stosuje się czasem w celu ominięcia mechanizmu wywołań wirtualnych, ale to dość zaawansowany temat.

W powyższym kodzie jest jeszcze jeden ciekawy szczegół. Jeden z konstruktorów, 

```c++
 Wektor(size_t new_size, int init_value = 0);
```

posiada argumenty z wartościami domyślnymi. Jeżeli zadeklarujemy je w pliku nagłówkowym, to nie możemy tej deklaracji powtórzyć w pliku źródłowym 

```c++
Wektor::Wektor(size_t new_size, int init_value) { ... }
```

### Plik testowy

Testy naszej klasy (lub jakikolwiek kod ją używający) umieszczamy w osobnym pliku, np. `main7.cpp`:

```c++
#include "vector7.h"
#include <iostream>

int main()
{
    Wektor v(4);
    Wektor w(6);

    w.front() = 7;
    w[1] = 1;
    w[2] = 2;
    v = w;
    std::cout << w[0] << " " << w[1] << "\n";
    std::cout << w.size() << "\n";
    std::cout << w.empty() << "\n";
    std::cout << w.Wektor::empty() << "\n";   // dziwactwo, nie pisz tak
}
```

Najważniejsze to zapamiętać, że w każdym pliku używającym klasy należy dołączyć jej interfejs:

```c++  
#include "vector7.h"
```

### Kompilacja

Programy podzielone na pliki można kompilować ręcznie lub za pomocą specjalnych narzędzi. 

- Kompilacja ręczna

  W naszym przypadku w systemie Linux można nasz podzielony na części program skompilować poleceniem 

   ```c++
  g++ main7.cpp vector7.cpp
   ```

  Kompilator utworzy plik wykonywalny o nazwie `a.out`. Kompilacji podlegają wyłącznie pliku źródłowe (`*.cpp`), nigdy nie kompilujemy plików nagłówkowych (`*.h`). 

- Kompilacja w systemie CMake

  W tym przypadku w pliku `CMakeLists.txt` umieszczamy stosowna instrukcję `add_executable`, np.

  ```cmake
  add_executable(wektor7 main7.cpp vector7.cpp vector7.h)
  ```

  Program można skompilować poleceniem `cmake --build <build-directory>`, np. 

  ```bash
  cmake --build .
  ```

  wydanym z katalogu, w którym chcemy zbudować program.

## Funkcje `inline`

Przedstawiony powyżej sposób podziału na interfejs i implementację nie jest jedyny. Często bezpośrednio w interfejsie klasy umieszcza się definicje krótkich funkcji, np.

```c++ 
class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;

  public:
    Wektor() {}    
    Wektor(size_t new_size, int init_value = 0);
    Wektor(const Wektor& v);
    ~Wektor();
   
    Wektor& operator=(const Wektor& rhs);
    int& operator[](size_t index);
    int operator[](size_t index) const;
    int& at(size_t index);
    int at(size_t index) const;
    int& front() { return _dane[0]; }              
    int front() const { return _dane[0]; }         
    int& back() { return _dane[_size - 1]; }       
    int back() const { return _dane[_size - 1]; }  
    size_t size() const { return _size; }          
    int* data() const { return _dane; }            
    bool empty() const { return _size == 0; }      
};
```

Ułatwia to optymalizację kodu. Kompilator wszystkie takie funkcje składowe potraktuje tak, jakby zadeklarowano je z omówionym już  deklaratorem `inline`.

Definicje funkcji można także umieścić w pliku nagłówkowym poza definicją klasy (zwykle tuż pod nią). Trzeba tylko pamiętać przestrzeni nazw i jawnym deklaratorze `inline`:

```c++
class Wektor
{
    int* _dane = nullptr;
    size_t _size = 0;

  public:
    Wektor();
    Wektor(size_t new_size, int init_value = 0);
    Wektor(const Wektor& v);
    ~Wektor();

    Wektor& operator=(const Wektor& rhs);
    int& operator[](size_t index);
    int operator[](size_t index) const;
    int& at(size_t index);
    int at(size_t index) const;
    int& front();
    int front() const;
    int& back();
    int back() const;
    size_t size() const;
    int* data() const;
    bool empty() const;
};

inline bool Wektor::empty() const { return _size == 0; }

// i tak dalej z innymi krótkimi funkcjami 
```

