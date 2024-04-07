## Konstruktor i destruktor

### Konstruktory

Jednym z problemów, jakie stwarza praca ze strukturami, jest możliwość zapomnienia o konieczności zainicjalizowania jego składowych przed pierwszym użyciem całej struktury. Wiemy także, że w odróznieniu od struktur, klasy umożliwiają definiowanie funkcji składowych. Czy jest możliwe zdefiniowanie funkcji składowej, której zadaniem byłaby inicjalizacja obiektu (wprowadzenie go w dobrze określony stan początkowy) i która wywoływana byłaby automatycznie, w momencie tworzenia obiektu, czyli przed jego pierwszym użyciem? Odpowiedź jest pozytywna. Takie funkcje nazywamy konstruktorami. Sposób ich użycia ilustruje przykład:

```c++    
class Wektor
{
    int* dane;
    size_t size;
    
  public:
    Wektor() // konstruktor (bezargumentowy, domyślny)
    {
        dane = nullptr;
        size = 0;
    }
};

int main()
{
    Wektor w;
}
```

Konstruktor charakteryzuje się tym, że jego nazwa jest tożsama z nazwą klasy. Skoro klasa nazywa się `Wektor`, to i konstruktor wygląda jak funkcja nazywająca się `Wektor`. Pod tym względem C++ nieco różni się np. od języka Python, w którym odpowiednik konstruktora ma ustaloną nazwę `__init__`. 

Kolejną cechą konstruktora jest to, że on nigdy nie zwraca wartości. Czyli tak, jakby typ jego wyniku definiowany był zawsze jako `void`. Skoro jednak tam zawsze jest `void`, to upraswzczamy sobie życie i nigdy tego `void` nie piszemy (a nawet jest to zabronione). Stąd definicja konstruktora bezargumentowgo w klasie `Wektor` wygląda tak: 

```c++    
 Wektor()
 {
     // jakiś kod
 }
```

Konstruktory wyróżnia też to, że nie można ich wywołać, stosując notację "z kropką" czy "ze strzałką".   Kompilator wywołuje je automatycznie podczas tworzenia obiektu:

```c++
Wektor w;
```

Można je też wywoływać jak funkcję, tworzącą obiekt nienazwany:

```c++
Wektor(); // tworzy wektor nienazwany o czasie życia ograniczonym do 1 instrukcji
```

co w tej chwili potraktujmy jako ciekawostkę. Obiekty nienazwane mają bardzo krótki czas życia (jedna instrukcja) i stosuje się je np. w funktorach (o czym napiszę później).

Konstruktorów może być kilka. Do powyższego konstruktora domyślnego można dołączyć kolejny:

```c++
Wektor(size_t new_size, int init_value = 0)
{
    size = new_size;
    dane = new int [new_size];
    for (size_t i = 0; i < size; i++)
    {
        dane[i] = init_value;
    }        
}
```

Konstruktor ten można wywołać np. tak:

```c++
Wektor w(10, -1); // 10 elelemntów, każdy o wartości -1
```

Pierwszy jego argument niesie informację o liczbie elementów, jakie ma przechowywać właśnie tworzony wektor (= obiekt klasy `Wektor`), a drugi - jaka ma być wartość początkowa każdego z nich. 

Bardzo często definiuje się też **konstruktory kopiujące** (ang. *copy constructors*), które tworzą nowy obiekt jako kopię już istniejącego. W naszym przykładzie implementacja takiego konstruktora może wyglądać następująco:

```c++
Wektor(const Wektor& v)
{
    size = v.size;                    // kopiowanie rozmiaru wektora
    dane = new int [size];            // alokacja pamięci na dane
    for (size_t i = 0; i < size; i++) 
    {
        dane[i] = v.dane[i];          // kopiowanie danych
    }        
}
```

Uruchamia się go np. tak:

```c++
Wektor w(10, -1);   // Wektor 10 liczb typu int o wartości początkowej -1
Wektor v(w);        // v jest kopią w
```

### Destrukktor

W rozwiązaniu nie używającym klas do zwolnienia pamięci zajmowanej przez `Wektor` używałem napisanej przez siebie funkcji `free`. Czy można to zautomatyzować? Tak. W Języku Python taka funkcja, znana w programowaniu obiektowym jako destruktor, nosi stałą nazwę `__del__`. W języku C++ destruktor ma nazwę taką jak konstruktor z dodanym na początku znakiem tyldy (`~`):

```c++    
~Wektor() // destruktor klasy Wektor
{
    delete [] dane;
    dane = nullptr;
    size = 0;        
}
```

Destruktor, wbrew swojej nazwie, nie "niszczy" obiektu. On zwykle zwalnia zasoby zajmowane przez obiekt, przygotowując program do zwolnienia zajmowanej przezeń pamięci. W naszym przykładzie destruktor zwalnia pamięć operatorem `delete[]`, a przy okazji zeruje składową `size` i ustawia wskaźnik na `nullptr`. W pewnym sensie destruktor odwraca to, co zrobił konstruktor. Jeśli konstruktor alokuje pamięć operatorem `new`, to destruktor zwolni ją operatorem `delete`. Jeżeli konstruktor otwiera plik, to zwykle destruktor go zamyka.   

Destruktotra w zasadzie nie wywołuje się ręcznie - jest  to możliwe, ale nigdy nie spotkałem się z taką praktyką. Destruktor jest wywoływany automatycznie tuż przed zwolnieniem pamięci zajmowanej przez obiekt. W poniższym przykładzie

```c++
int main()
{
    Wektor w(5);
} <--- na tej klamrze zostanie wywołany destruktor obiektu w
```

destruktor obiektu `w` zostanie wywołany na klamrze kończącej zakres, w którym go zdefiniowano.

Każda klasa może mieć dowolną liczbę konstruktorów (w tym zero) i co najwyżej jeden destruktor. 

### Zakres i czas życia obiektu

W tym miejscu wypada powiedzieć coś o zakresie (ang. *scope*) i czasie życia obiektów. **Zakres** to po prostu fragment kodu otoczony klamrami.

```c++      
if (x < 0)
{ 
    Wektor w;              // wywoła konstruktor obiektu w
    if (x < 10)
    {
        Wektor y(100);     // wywoła konstruktor obiektu y
        Wektor z(11, -1);  // wywoła konstruktor obiektu z  
    } // <-- tu wywołają się destruktory obiektów z i y 
    Wektor v;              // wywoła konstruktor obiektu v
} // <-- tu wywołają się destruktory obiektów v oraz w
```

W powyższym przykładzie widzimy dwa zakresy, "zewnętrzny" i "wewnętrzny". Obiekty `w` i `a` zostały zdefiniowany w zakresie zewnętrznym, więc ich destruktory wywołają się na klamrze kończącej ten zakres. Obiekty `y` i `z` zdefiniowano w zakresie wewnętrznym i ich destruktory wywołają się na klamrze wewnętrznej. 

Czas życia obiektu rozciąga się od chwili wywołania jego konstruktora do momentu wywołania jego destruktora.

Wszystkie obiekty lokalne w funkcjach istnieją więc w zakresie tych funkcji. W szczególności, wszystkie one giną po zakończeniu działania funkcji. 

Istnieją obiekty, które nie należą do żadnego zakresu. Są to tzw. obiekty globalne (innym przykładem są też obiekty statyczne). Znanymi przykładami obiektów globalnych są `std::cin` i `std::cout`. Ich konstruktory wywoływane są przed wejściem programu do funkcji `main`, a destruktory - po wyjściu z tej funkcji. Dlatego można ich używać w całym programie (z wyjątkiem, być może, konstruktorów i destruktorów obiektów globalnych - czy rozumiesz, dlaczego?).

### Krótkie podsumowanie

Po dodaniu konstruktorów i destruktorów nasz kod wyglądać może tak:

```c++
#include <iostream>

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
    
    Wektor(size_t new_size, int init_value = 0)
    {
        size = new_size;
        dane = new int[new_size];
        for (size_t i = 0; i < size; i++)
        {
            dane[i] = init_value;
        }        
    }
    
    Wektor(const Wektor & v)
    {
        size = v.size;
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
    Wektor w(5); // test
}
```