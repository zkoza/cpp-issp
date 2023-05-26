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
    
  - **Instrukcja `switch`** używana jest, gdy chcemy wykonać różne operacje zależnie od wartości pewnej zmiennej całkowitoliczbowej. Przykład:

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

  Instrukcje iteracyjne (nazwa nieformalna: *pętle*) to instrukcje, które służą do wielokrotnego wykonania tego samego kodu. W języku C++ wyróżnia się cztery rodzaje tego rodzaju instrukcji: pętla `while`, pętla  `do-while`,  klasyczna pętla `for` i pętla `range for`, czyli pętla `for` z zakresem. Oto ich zwięzłe omówienie:

  - **Instrukcja `while`** używana jest do wykonywania jakiejś instrukcji (zwykle blokowej) tak długo, jak długo prawdziwe jest wyrażenie sterujące tą pętlą. Przykład użycia:

    ```c++
    #include <iostream>
    int main()
    {
        int n = 19;
        std::cout << "liczba n w układzie dwójkowym czytana w odwróconej kolejności:\n";
        while(n != 0)
        {
            std::cout << ((n & 1) == 0 ? 0 : 1); // wyświetl ostatni bit
            n >>= 1; // przesuń reprezentację bitową w prawo o jedną pozycję
        }
    }
    ```

    Wyrażenie warunkowe sterujące umieszczane jest w nawiasach okrągłych tuż po słowie kluczowym `while`.  Powyższy program daje następujący wynik:
    ```txt
    liczba n w układzie dwójkowym czytana w odwróconej kolejności:
    11001
    ```

    Oczywiście, 19 w układzie dwójkowym ma reprezentację `0b10011`, bo `19 == 1 + 2 + 16`. 

    Zdarza się dość często, że  wyrażenie warunkowe nie ma postaci wyrażenia logicznego, któe, która w sposób jawny byłoby wyrażeniem warunkowym o wartości `false` lub `true` (rozpoznajemy je po występowaniu operatora lub funkcji zwracającej wartość typu `bool`, np. `n < 0` lub `!tab.empty() || tab[0] == 0`) . Możliwe są dwa przypadki. W pierwszym, wyrażenie sterujące pętlą ma postać wyrażenia arytmetycznego, czyli takiego, którego wartością jest liczba, np.
    ```c++
    while (n)
    {
        std::cout << n-- << ' ';
    }
    ```

    W takich przypadkach kompilator dokona niejawnej konwersji wyrażenia arytmetycznego do wyrażenia logicznego, zamieniając każdą wartość różną od zera na `true`, a zero na `false`. Innymi słowy, powyższa pętla jest równoważna następujęcej:

    ```c++
    while (n != 0)
    {
        std::cout << n-- << ' ';
    }
    ```

    Powyższa sposób jest bardzo często wykorzystywany w programowaniu w językach C/C++ i koniecznie należy go zapamiętać. Drugi przypadek to  niejawna konwersja wyrażenia do typu `bool`. Przykład dość typowy:

    ```c++
    #include <iostream>
    #include <fstream>
    int main()
    {
        int n;
        std::ifstream in("dane_we.txt");
    	while (in >> n)                  // idiom warty zapamiętania
    	{
        	std::cout << n << "\n";
    	}
    }
    ```

    Powyższy program wczytuje z pliku `dane_we.txt` liczby całkowite tak długo, aż w strumieniu pojawi się błąd. W ten sposób można z poprawnie przygotowanych plików wczytywać wszystkie dane: próba wczytania danych nieistniejących zakończy się ustawieniem strumienia w stan `fail`, co z kolei spowoduje przerwanie powyższej pętli przed uruchomieniem sterowanej nią instrukcji blokowej. Jak to działa? Otóż trzeba wiedzieć, że `operator>>` w wyrażeniu ` in >> n` zwraca referencję do swojego lewego argumentu, tu: do `in`. Kompilator zobaczy więc następujące  wyrażenie warunkowe: `while(in)`.  Następnie zauważy, że w klasie, do której należy ``in`, zdefiniowano `operator bool`, którego można użyć do konwersji wyrażenia `in` na `bool`, czyli na `true` lub `false`. Wyrażenie to równoważne będzie czemuś w rodzaju `while(!in.fail())`, czyli "dopóki strumień nie znajduje się w sygnalizującym błąd stanie `fail`" . 

  - **Instrukcja `do-while`** działa podobnie do instrukcji `while`, tyle że warunek kontynuowania pętli sprawdzany jest po wykonaniu iterowanej instrukcji. Przykład:

    ```c++
    #include <algorithm>
    #include <iostream>
    #include <string>
     
    int main()
    {
        std::string s = "abc";
        std::sort(s.begin(), s.end());
     
        do 
        {       
            std::cout << s << '\n';
        }while (std::next_permutation(s.begin(), s.end()));
    }
    ```

    Powyższy program wyświetla, zachowując porządek leksykograficzny, wszystkie permutacje zbioru trójelementowego:

    ```txt
    abc
    acb
    bac
    bca
    cab
    cba
    ```

    Powyższa pętla zostanie przerwana wtedy, gdy `std::next_parmutation` zwróci wartość `false`, co z kolei ma miejsce, gdy `s` ma największą możliwą "wartość", tzn. gdy nie istnieje "kolejna permutacja".

  - Klasyczna **instrukcja `for`** jest jedną z najbardziej popularnych instrukcji iteracyjnych w C++.  Prosty przykład instrukcji wyznaczającej sumę liczb naturalnych od 1 do 5:

    ```c++
    int suma = 0;
    int n = 5;
    for (int i = 1; i < n; i++)
        suma += i;
    ```

    Jak widać, wyrażenie sterujące klasycznej pętli `for` składa się z 3 części oddzielonych średnikami. Jeżeli w nieformalny sposób pętlę tę zapiszemy w postaci

    ```c++
    for (wyrażenie-lub-deklaracja; warunek; wyrażenie)
        instrukcja;
    ```

    to spostrzeżemy, że 

    - pierwsza część wyrażenia sterującego zawiera jakieś wyrażenie lub deklarację, które jest wykonywane dokładnie raz na samym początku pętli. Przykład z wyrażeniem:

      ```c++ 
      for (i = 0; ; ) rób_coś();
      ```

      Przykład z deklaracją:

      ```c++ 
      for (int i = 0; ; ) rób_coś();
      ```

      Przykład złożonej deklaracji dwóch zmiennych:

      ```c++
      for(int i = 0, j = 0; ; ) rób_coś();
      ```

    - druga część wyrażenia sterującego zawiera warunek kontynuowania pętli, np.

      ```c++
      for ( ; i > 0; ) rób_coś(i);
      ```

      Jeżeli warunku nie ma, to kompilator zamieni go na `true`. Czyli pętla 

      ```c++
      for (int i = 0; ; i++) rób_coś(i);
      ```

      jest równoważna pętli nieskończonej

      ```c++
      for (int i = 0; true; i++) rób_coś(i);
      ```

    - trzecia część to dowolna instrukcja wykonywana *po* instrukcji stanowiącej treść pętli, np.:

      ```c++
      for ( ; ; i++) rób_coś(i);
      ```

       lub 

      ```c++ 
      for ( ; ; x += 9.0) rób_coś(x); // średnio dobry pomysł
      ```

      czy nawet instrukcja zawierająca operator przecinkowy (tu: oddzielający `i++` od `j -= 2`):

      ```c++
      for( ; ; i++, j -=2) rób_coś(i, j);
      ```

    Każda z tych części może być pusta. W szczególności, pusta pętla `for`

    ```c++
    for ( ; ; )
    ```

     jest równoważna pętli nieskończonej

    ```c++
    while(true)
    ```

  - **Pętla `for` z zakresem**  służy do operacji na całych kontenerach.  Na przykład, dla kontenera

    ```c++
    std::vector<int> v;
    ```

    można w następujący sposób wyświetlić jego zawartość:

    ```c++
    for (int n: v) std::cout << n << " ";
    ```

    Czytamy to w następujący sposób: "pobieraj kolejne elementy kontenera `v`, od jego pierwszego elementu (wskazywanego przez `v.begin()`, o ile istnieje) do elementu ostatniego, zapisz je w zmiennej `n` typu `int`, po czym wykonaj treść instrukcji `for` (tu: `std::cout << n << " ";`)" .  Zwykle nie podaje się typu zmiennej zawierającej kolejne elementy kontenera, w związku z czym powyższa instrukcja w rzeczywistym kodzie prawdopodobnie zostałaby zapisana tak:

    ```c++
    for (auto n: v) std::cout << n << " ";
    ```

    Powyższy sposób pozwala w wygodny sposób odczytywać elementy kontenera, *kopiując* je kolejno do zmiennej `n`. Jeżeli kolejne element chcemy modyfikować, stosujemy referencję:

    ```c++
    for (auto & n: v) n = 0;  // zeruje wszystkie elementy kontenera v
    ```

    W powyższym przykładzie `n` jest referencją do kolejnych elementów kontenera `v`, dlatego możemy je za jej pośrednictwem modyfikować. Stosuje się także dostęp do kolejnych elementów poprzez stałą referencję.   Na przykład dla listy wektorów

    ```c++
    std::list<std::vector<double>> lista;
    ```

     można w ten sposób w efektywny sposób wyświetlić długości każdego z wektorów przechowywanych na licie:

    ```c++
    for (const auto& vec: lista) std::cout << vec.size() << ' '; 
    ```

    Pętla `for` bez referencji byłaby nieefektywna, gdyż poszczególne wektory byłby wówczas kopiowane do zmiennej `vec` przed jej każdorazowym użyciem w ciele instrukcji `for`.

    Warto na koniec wspomnieć o tzw. deklaracji wiązania strukturalnego (ang. [*structured binding declaration*](https://en.cppreference.com/w/cpp/language/structured_binding)):

    ```c++
    #include <iostream>
    #include <map>
    int main()
    {
        std::map<std::string, int> mapa = {{"ala", 1}, {"ola", 3}};
    	for (auto [key, value] : mapa)
        	std::cout << key << " -> " << value << "\n";
    }
    ```

    W powyższym przykładzie mamy do czynienia z kontenerem `mapa` typu  `std::map`, który przechowuje pary wartości, `std::string` i `int`. Każdemu z tych elementów możemy przypisać dowolną nazwę (tu: `key` i `value`).  Bez tego ułatwienia składniowego, powyższą pętlę musielibyśmy zapisać w mniej czytelny sposób:

    ```c++
    	for (auto para: mapa)
        	std::cout << para.first << " -> " << para.second << "\n";
    ```

    który wymaga szczegółowej znajomości typu zwracanego przez `operator*` zastosowany na iteratorze do kontenera `mapa` (tu: `std::pair<std::string, int>`) .

    W każdym z powyższych przypadków program zwraca

    ```
    ala -> 1
    ola -> 3  
    ```

- **Instrukcje etykietowane** (*labeled statements*) - służą organizacji przepływu kodu w instrukcji `switch` i instrukcji `goto`. Należą do nich instrukcje `case` i `default` z opisanej powyżej instrukcji `swotch`, a także deklaracja miejsca docelowego instrukcji `goto`:

  ```C++
  #include <iostream>
  int main()
  {
      int n = 0;
  start:                     // instrukcja etykietowana
      std::cin >> n;
      if (n != 1) 
      {
          std::cout << "wpisano niepoprawną liczbę, spróbuj jeszcze raz\n";
          goto start;
      }
  }
  ```

  Oczywiście powyższy program znacznie lepiej byłoby zapisać przy pomocy instrukcji `while` lub `do-while`. 

- **Instrukcje skoku** (*jump statements*), obejmują instrukcje `continue`, `break`, `goto` i `return` i służą do zmiany naturalnej kolejności  wykonywania instrukcji 

  - **instrukcja `continue`** przerywa wykonywanie bieżącej iteracji pętli `for`, `while` lub `do-while`. 

    ```c++
    for (int i = 0; i < 10; i++)
    {
        if ( i % 2 == 0) continue;
        std::cout << "i = " << i << "\n";
    }
    ```

    wynik:

    ```c++
    i = 1
    i = 3
    i = 5
    i = 7
    i = 9
    ```

  - **instrukcja `break`** przerywa wykonanie całej pętli `for`, `while` lub `do-while`. 

    ```c++
    for (int i = 0; i < 10; i++)
    {
        if ( i * i > 10) break;
        std::cout << "i = " << i << "\n";
    }
    ```

    pętla zostanie przerwana po dojściu do `i == 4`

    ```txt
    i = 0
    i = 1
    i = 2
    i = 3
    ```

  - **instrukcja `goto`** to instrukcja , którą stosuje się niezwykle rzadko. Przykład jej użycia znajduj się w poprzednim podpunkcie ("instrukcje etykietowane"). 

- **Bloki `try`** (*try blocks*) używane są do wyłapywania wyjątków i zostaną omówione razem z wyjątkami. 