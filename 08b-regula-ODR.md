#### Reguła ODR

W C++ obowiązuje tzw. ***reguła ODR*** (ang. *one definition rule*): w całym programie może wystąpić tylko jedna definicja danej funkcji. Z kolei liczba deklaracji jest nieograniczona, z tym że przed pierwszym użyciem, funkcja musi być zadeklarowana (przy czym definicja funkcji jest także jej deklaracją). 

Jeśli więc poniższą funkcję `suma` 

```c++
int suma(int n, int m)
{
    return n + m;
}
```

chcemy wykorzystać np. w funkcji `main`, to mamy trzy wyjścia:

- Umieścić **definicję** funkcji `suma` **przed** definicją funkcji `main`, np.:

  ```c++
  #include <iostream>
  
  int suma(int n, int m)
  {
      return n + m;
  }
  
  int main()
  {
      std::cout << suma(1, 3) << "\n"; // OK, suma została wcześniej zadeklarowana
  }
  ```

- Umieścić **deklarację** funkcji `suma` **przed** definicją funkcji `main`, a definicję funkcji `suma` umieścić gdzieś dalej w tym samym pliku:

  ```c++
  #include <iostream>
  
  int suma(int n, int m);  // deklaracja
  
  int main()
  {
      std::cout << suma(1, 3) << "\n";  // OK: suma została wcześniej zadeklarowana
  }
  
  int suma(int n, int m)   // definicja
  {
      return n + m;
  }
  ```

- Umieścić ***deklarację*** funkcji `suma` **w osobnym pliku** (nagłówkowym), **włączyć** go na początku pliku zawierającego definicję funkcji `main` makrem `#include`, umieścić ***definicję*** funkcji `suma` w osobnym pliku (źródłowym), i w końcu umieścić definicję funkcji `main`  w jeszcze innym pliku (też źródłowym):

  ```c++
  // Plik suma.h, uproszczony (bez tzw. strażnika nagłówka)
  int suma(int n, int m);  // deklaracja
  ```

   ```c++
  // Plik suma.cpp
  #include "suma.h"  // to nie jest tu konieczne, ale jest w dobrym tonie
  
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
  
  Aby skompilować tak pofragmentowany program, trzeba przekazać kompilatorowi informację, że program składa się z wielu części. W przypadku tak małych projektów wystarczy komenda w rodzaju 
  
  ```bash 
  > g++ main.cpp suma.cpp
  ```
  
  W przypadku dużych projektów stosuje się systemy budowania, np. cmake. Prosty skrypt cmake (`CMakeLists.txt`) odpowiadający powyższemu projektowi wyglądać może tak:
  
  ```cmake
  cmake_minimum_required(VERSION 3.10)
  project(moj_projekt_akademicki LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  
  add_executable(suma main.cpp suma.cpp) # cel "suma" zależy od main.cpp i suma.cpp
  ```

Istnieje specjalny rodzaj funkcji, które nie podlegają regule ODR. Są to tzw. funkcje otwarte *(inline*), o których szerzej napisze w innym miejscu.  