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

- *declaration statements*  (instrukcje deklaratywne)

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

- *expression statements* (instrukcje - wyrażenia)

  Jest to dowolne wyrażenie arytmetyczne zakończone średnikiem.  Przykłady:
  ```c++
  a = 9;          // operator przypisania
  x = sin(8.0);   // operator przypisania 
  a++;            // operator inkrementacji 
  std::cout << 7; // operator <<
  8;              // kompilator zignoruje tę instrukcję
  ;               // pusta instrukcja, ignorowana przez kompilator
  print(7);       // wywołanie funkcji
  ```

- *compound statements* (instrukcje blokowe)

  Instrukcja blokowa to po prostu ujęty w klamry blok obejmujący co najmniej jedną instrukcję

  ```c++
  if (n >= 0)
  {                   // początek instrukcji blokowej
      m += n;
      std::cout << "m = " << m << "\n";
  }                   // koniec instrukcji blokowej
  ```

- *selection statements* (instrukcje wyboru)

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
    - Wersja z inicjalizatorem:
    
    ```c++
    std::string s = "To jest wrocław";
    if (auto n = s.find("wrocław"); n != std::string::npos)
    {
        s.replace(n, sizeof("wrocław") - 1, "Wrocław");   // -1 pomija bajt zerowy
    }
    std::cout << s << "\n"; // wyświetli "To jest Wrocław" 
    ```
    
    Wersja z inicjalizatorem służy do wprowadzenia zmiennej (tu: `n`), której można użyć w warunku i w treści instrukcji sterowanych tą instrukcją `if`, ale która nie będzie widoczna poza instrukcją `if`.    
    
  - Instrukcja `switch`

    Przykład:

    ```c++
    #include <iostream>
    
    int main()
    {
        int i;
        std::cin >> i;
        switch (i)
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

  - Instrukcje `if constexpr`, `if consteval`  - temat zaawansowany (tu pomijam) 

- iteration statements

- labeled statements

- jump statements

- try blocks