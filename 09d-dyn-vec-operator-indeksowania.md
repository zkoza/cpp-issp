## Operator indeksowania i składowa at

Jak dotąd nasz `Wektor` jest wyjątkowo niefunkcjonalny: mozemy go zainicjalizować, poprawnie zniszczyć, ale nie potrafimy nawet odczytać jego elementów, cóż dopiero je zmieniać!

### Składowa at() 

Powyższemu problemowi łatwo zaradzić i mamy nawet dwa rozwiązania. Zaczniemy od łatwiejszego w implementacji, choć nieco trudniejszemu w użyciu - zdefiniowaniu funkcji wewnątrz klasy. Na przykład tak:

```c++
class Wektor
{
    ...
	int& at(size_t index)
	{
    	if (index >= size) throw std::out_of_range("Index out of range in Wektor::at");
    	return dane[index];
	}
};
```

Skoro `at` jest  funkcją składową, to używamy jej za pomocą "notacji z kropką":

```c++
int main()
{
    Wektor w(5);                   // konstrukcja wektora
    w.at(1) = 1;                   // zmiana wartości wektora
    std::cout << w.at(1) << "\n";  // odczyt wartości wektora
}
```

Jak to działa? Funkcja `at` pobiera jeden argument, typu `size_t` (jest to typ zaprojektowany specjalnie do używania jako indeks tablic, w praktyce jest to zwykle `uint64_t`, czyli 64-bitowa liczba całkowita bez znaku). Następnie sprawdza, czy indeks mieści się w rozmiarze tablicy. Jeśli nie, to zgłasza wyjątek instrukcją `throw`.  Wyjątkom poświęcę osobne rozważania, tu wystarczy wiedzieć, że (niewyłapane) zgłoszenie wyjątku powoduje natychmiastowe przerwanie wykonywania danej funkcji i wymaga specjalnej obsługi w innej części programu - jest to standardowy w C++ sposób reakcji na błędy i/lub nieoczekiwane sytuacje (np. wyczerpanie miejsca na dysku). W kolejnym wierszu funkcja zwraca odpowiedni element tablicy. Kluczowa obserwacja: powyższa funkcja `at` zwraca swoją wartość przez referencję. Oznacza to, że wyrażenie

```c++
w.at(1)
```

  zostanie przez kompilator zinterpretowany tak, jakbyśmy napisali 

```c++    
w.dane[index]
```

a (pomijając kwestię podziału na sekcje prywatne i publiczne) takie wyrażenie można użyć z lewej strony operatora przypisania, prawda? Dlatego poprawna jest instrukcja 

```c++
w.at(1) = 1;      
```

w której funkcję wywołuje się z lewej strony operatora przypisania. Możliwość pisania takiego kodu to główny powód stosowania funkcji zwracających swoją wartość przez referencję. Funkcje takie dość powszechnie występują w programowaniu obiektowym, gdyż umożliwiają wygodną zmianę stanu obiektów.  

### Operator indeksowania

Funkcja składowa `at` jest łatwa do napisania, jednak dość niezręczna w użyciu. Wyobraźmy sobie bowiem np, tablicę trójwymiarową. Jej indeksowanie wymagałoby pisania czegoś w rodzaju `tab.at(4).at(5).at(8)`. Wolelibyśmy zamiast tego widzieć po prostu `tab[4][5][8]`, nieprawdaż? Można temu łatwo zaradzić, definiując w klasie `Wektor`  operator indeksowania, czyli `operator[]` 

```c++
class Wektor
{
    ...
    int& operator[](size_t index)
    {
        return dane[index];
    }
};
```

Jego treść bardzo przypomina treść funkcji składowej `at`, jedyną różnicą jest brak weryfikacji poprawności indeksu. Skąd ta różnica? Otóż dokładnie tak to rozwiązano w bibliotece standardowej C++: znajduje się w niej klasa (szablonowa) `std::vector`, w której zdefiniowanio zarówno składową `at`, jak i `operator[]`, przy czym standard gwarantuje, że składowa `at` sprawdza poprawność indeksu (por. https://en.cppreference.com/w/cpp/container/vector/at, *`at(pos)` returns a reference to the element at specified location `pos`, with bounds checking. If pos is not within the range of the container, an exception of type [std::out_of_range](https://en.cppreference.com/w/cpp/error/out_of_range) is thrown*), standard nie narzuca jednak żadnych ograniczeń na implementację `operator[]`. W praktyce `operator[]` nie sprawdza poprawności indeksowania, jednak istnieją sposoby, by go do tego skłonić, dlatego składową `at` w rzeczywistym kodzie widuje się rzadko. 

Sprawdźmy, jak działa nasz operator:

```c++       
int main()
{
    Wektor w(5);
    w[1] = 8;                   // zapis
    std::cout << w[1] << "\n";  // odczyt
}
```

Wszystko działa zgodnie z oczekiwaniami. Sprawdźmy jeszcze co się stanie, gdy przekroczymy zakres indeksowania tablicy:

```c++   
int main()
{
    Wektor w(5);
    w[1] = 8;
    std::cout << w[1] << "\n";
    std::cout << w[5] << "\n"; // tu jest błąd, program zapewne go nie zauważy 
    w.at(1) = 1;
    w.at(10) = -1;   // tu program zgłosi wyjątek
}
```

Program przechodzi przez instrukcję ` std::cout << w[5] << "\n"`  i nie zgłasza błędu, gdyż `operator[]` nie weryfikuje poprawności indeksowania (ponieważ `w` zdefiniowano jako `Wektor` 5 elementów, można go indeksować wyłącznie indeksami 0, 1, 2, 3 lub 4). Błąd zgłosi dopiero instrukcja `w.at(10) = -1`. Program "padnie" z komunikatem 

```txt
terminate called after throwing an instance of 'std::out_of_range'
  what():  Index out of range in Wektor::at(size_t)
```

Jeśli indeks w operatorze `[]` *znacznie* przekroczy dopuszczalny zakres, to program otrzyma sygnał `segfault` (*segmentation fault*), co w praktyce oznacza jego pad. Nie ma żadnych gwarancji co do tego, co w tym kontekście znaczy słowo "znacznie". 

Podsumowując, nasz program wyglądać teraz może tak:

```   c++
#include <iostream>
#include <stdexcept>  // dla std::out_of_range

class Wektor
{
    int* dane = nullptr;
    size_t size = 0;

  public:
    Wektor()
    { }

    Wektor(size_t new_size, int init_value = 0)
        : size(new_size)  // <--- lista inicjalizacyjna (preambuła) konstruktora
    {
        dane = new int[size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = init_value;
        }
    }

    Wektor(const Wektor& v)
        : size(v.size)   // <--- lista inicjalizacyjna (preambuła) konstruktora
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

    int& operator[](size_t index)
    {
        return dane[index];
    }

    int& at(size_t index)
    {
        if (index >= size) 
            throw std::out_of_range("Index out of range in Wektor::at(size_t)");
        return dane[index];
    }

};

int main()
{
    Wektor w(5);
    w[1] = 8;
    std::cout << w[1] << "\n";
    std::cout << w[5] << "\n"; // tu jest błąd, program zapewne go nie zauważy
    w.at(1) = 1;
    w.at(10) = -1;             // tu program zgłosi wyjątek
}
```

