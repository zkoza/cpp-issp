## Instrukcje

Instrukcje to fragmenty programu, które są wykonywane kolejno, jedna po drugiej. Instrukcje składają się na treść funkcji. 

```c++
int main()
{
 start:              // instrukcja etykiety
    int n = 100;     // instrukcja deklarująca
    n++;             // instrukcja wartościująca
    if (n == 0)      // instrukcja wyboru
    {                // instrukcja blokowa
         goto start; // instrukcja skoku 
    }
    return 0;        // instrukcja zakończenia funcji 
}
```

Standard języka C++ wyróżnia następujące typy instrukcji

- **Instrukcje deklaratywne** *(declaration statements*)

  Obejmują deklaracje i definicje zmiennych, typów, funkcji, szablonów, przestrzeni nazw. Temat-rzeka, gdyż C++ jest językiem z silną statyczną kontrolą typów. Kilka przykładów:

  ```c++
  int m = 90;    // Deklaracja zmiennej m typu int z wartością początkową
  auto p = &m;   // deklaracja p jako wskaźnika na int
  static int n;  // zmienna n typu int alokowana w bloku zmiennych static
  int & k = m;   // k jest referencją do m
  constexpr int f(int n) {return (n <= 0) ? 1 : n * f(n-1); } // funkcja f 
  auto zero = [](){ return 0; }  // zero jest wyrażeniem lambda
  using Real = double;           // Real jest aliasem typu double
  ```

- **Instrukcje-wyrażenia** (*expression statements*)

  Są to dowolne wyrażenia arytmetyczne zakończone średnikiem.  Przykłady:
  ```c++
  a = 9;          // operator przypisania
  x = sin(8.0);   // operator przypisania 
  a++;            // operator inkrementacji 
  std::cout << 7; // operator <<
  8;              // kompilator zignoruje tę instrukcję
  ;               // pusta instrukcja, ignorowana przez kompilator
  print(7);       // wywołanie funkcji
  ```

- **Instrukcje blokowe** (*compound statements*)

  Instrukcja blokowa to po prostu ujęty w klamry blok obejmujący co najmniej jedną instrukcję

  ```c++
  if (n >= 0)
  {                   // początek instrukcji blokowej
      m += n;
      std::cout << "m = " << m << "\n";
  }                   // koniec instrukcji blokowej
  ```

- **Instrukcje wyboru** (*selection statements*)

  Są to przede wszystkim instrukcje `if` i `switch`.  

  - **Instrukcja `if`** występuje w wersji samodzielnej, w wersji z `else` i w wersji z inicjalizatorem. 
    
    - Wersja samodzielna:
    
    ```c++
    if (n == 0)
        std::cout << "Tragedia";
    ```
    
    - Wersja z klauzulą `else`:
    
    ```c++
    if (m > 0)
        std::cout << "Dodatnia\n";
    else
        std::cout << "Ujemna lub zero\n";
    ```
    ​	Wersja z zagnieżdoną instrukcją `if-else`:
    
    ```c++
    if (m > 0)
        std::cout << "Dodatnia\n";
    else if (m < 0)
        std::cout << "Ujemna\n";
    else
        std::cout << "Zero\n"; 
    ```
    
    - Wersja z inicjalizatorem:
    
    ```c++
    std::string s = "To jest wrocław";
    if (auto n = s.find("wrocław"); n != std::string::npos)
    {
        s.replace(n, sizeof("wrocław") - 1, "Wrocław");   // -1 pomija bajt zerowy
    }
    std::cout << s << "\n"; // wyświetli "To jest Wrocław" 
    ```
    
    Wersja z inicjalizatorem służy do wprowadzenia zmiennej (tu: `n`), której można użyć w warunku i w treści instrukcji sterowanych daną instrukcją `if`, ale która nie będzie widoczna poza instrukcją `if`. Innymi słowy, w powyższym przykładzie zmiennej `n` można użyć w warunku wykonania instrukcji, `n != std::string::npos` oraz w instrukcji blokowej występującej tuż po tym warunku (w klamrach) i nigdzie indziej.
    
  - Instrukcja `switch` używana jest, gdy chcemy wykonać różne operacje zależnie od wartości pewnej zmiennej całkowitoliczbowej. Przykład:

    ```c++
    #include <iostream>
    
    int main()
    {
        int i;
        std::cin >> i;
        switch (i)  // zmienna sterująca musi być typu całkowitoliczbowego, np. int
        {
            case 1:
                std::cout << "1";
                break;
            case 2:              
                std::cout << "2";
                break
            case 3:
            case 4:    
                std::cout << "3 lub 4";
                [[fallthrough]]; // ucisza ostrzeżenie kompilatora o braku break-a
            case 5:
                std::cout << "5";
                break;           
            default:  // każdy inny "case":
                std::cout << "default";
        } 
        std::cout << '\n';
    }
    ```

    W powyższym przykładzie warto zwrócić uwagę na zastosowanie instrukcji `break`, która powoduje natychmiastowe wyjście z instrukcji `switch`.  Pominięcie jej w kodzie obsługującym poszczególne wartości zmiennej sterującej (tu: `i`) niemal zawsze jest błędem, a to dlatego, że wyrażenia `case` traktowane są jak etykiety instrukcji `goto` . Innymi słowy, jeżeli w powyższym przykładzie ze strumienia `std::cin` wprowadzimy wartość `i=2`,  to powyższy program "skoczy "  do etykiety `case 2:` i zacznie wykonywać odpowiadający jej kod (tu: `std::cout << "2";`). W powyższym przykładzie, po wykonaniu  `std::cout << "2"` program napotka instrukcję `break` i natychmiast wyjdzie z instrukcji `switch`, czyli do instrukcji `std::cout << '\n';`. Gdyby jednak użytkownik wprowadził wartość `i=3`, to program wykonałby trzy instrukcje:

    ``` c++
    std::cout << "3 lub 4";
    std::cout << "5";
    break;
    ```

    i dopiero teraz wyszedł z instrukcji `switch`. Powtórzę: pominięcie instrukcji `break` w każdym (z wyjątkiem ostatniego) bloku obsługującym konkretny "case" niemal zawsze jest błędem.  

    Można to zilustrować następująco. Instrukcja

    ```c++
    switch(i)
        case 0: 
            x++;
    		break;
    	case 1:
            y++;
            break;
    	case 2:
    		x += y;
            break;
        default:
    		y = 0;
    }
    ```

     jest równoważna instrukcji 

    ```c++
    if (i == 0)
        x++;
    else if (i == 1)
        y++;
    else if (i == 2)
        x += y;
    else
        y = 0;
    ```

    Natomiast jeżeli usuniemy z niej instrukcje `break`,

    ```c++
    switch(i)
        case 0: 
            x++;
    	case 1:
            y++;
    	case 2:
    		x += y;
        default:
    		y = 0;
    }
    ```

    to równoważny zestaw instrukcji może wyglądać następująco:

    ```c++
    if (i == 0)
        x++;
    if (i == 0 || i == 1)
        y++;
    if (i == 0 || i == 1 || i == 2)
        x += y;
    y = 0;  
    ```

  - Instrukcje `if constexpr`, `if consteval`  - temat zaawansowany (tu pomijam) 

- **Instrukcje iteracyjne** (*iteration statements*)

- **Instrukcje etykietowane** (*labeled statements*)

- **Instrukcje skoku** (*jump statements*)

- **Bloki `try`** (*try blocks*)