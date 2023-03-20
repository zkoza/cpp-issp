#### Reguła ODR

W C++ obowiązuje tzw. ***reguła ODR*** (ang. *one definition rule*): w całym programie może wystąpić tylko jedna definicja danej funkcji. Z kolei liczba deklaracji jest nieograniczona, z tym że przed pierwszym użyciem, funkcja musi być zadeklarowana (przy czym definicja funkcji jest także jej deklaracją). 

Jeśli więc powyższą funkcję `suma` chcemy wykorzystać np. w funkcji `main`, to mamy trzy wyjścia:

- Umieścić definicję funkcji `suma` przed definicją funkcji `main`, np.:

  ```c++
  #include <iostream>
  
  int suma(int n, int m)
  {
      return n + m;
  }
  
  int main()
  {
      std::cout << suma(1, 3) << "\n";
  }
  ```

- Umieścić deklarację funkcji `suma` przed definicja funkcji `main`, a po niej umieścić definicję funkcji `suma`:

  ```c++
  #include <iostream>
  
  int suma(int n, int m);  // deklaracja
  
  int main()
  {
      std::cout << suma(1, 3) << "\n";  // OK: suma została zadeklarowana
  }
  
  int suma(int n, int m)   // definicja
  {
      return n + m;
  }
  ```

- Umieścić deklarację funkcji `suma` w osobnym pliku, który włączymy na początku pliku zawierającego definicję funkcji `main`, umieścić definicję funkcji `suma` w osobnym pliku (źródłowym), i w końcu umieścić definicję funkcji `main`  w jeszcze innym pliku:

  ```c++
  // Plik suma.h, uproszczony (bez tzw. strażnika nagłówka)
  int suma(int n, int m);  // deklaracja
  ```

   ```c++
  // Plik suma.cpp
  #include "suma.h"
  
  int suma(int n, int m)
  {
      return n + m;
  }
   ```

  ```c++
  // Plik main.cpp
  #include "suma.h"        // tu wczytywana jest deklaracja funkcji suma
  #include <iostream>
  
  int main()
  {
      std::cout << suma(1, 3) << "\n";  // OK: suma została zadeklarowana
  }
  ```

Istnieje specjalny rodzaj funkcji, które nie podlegają regule ODR. Są to tzw. funkcje otwarte *(inline*).