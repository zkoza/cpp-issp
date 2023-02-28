## Operacje elementarne na typach wbudowanych

Skoro już wiemy, jak wewnętrznie reprezentowane są (czy też: mogą być) typy elementarne, pora na na jakąś akcję. Zacznijmy od tabeli skopiowanej z portalu [CppReference](https://en.cppreference.com/w/cpp/language/operator_arithmetic): 

![https://en.cppreference.com/w/cpp/language/operator_arithmetic](./img/03/common-operators.png)



Najczęściej używane podzielono w niej na 7 grup: operatory przypisania, inkrementacji i dekrementacji, arytmetyczne, logiczne, porównawcze, dostępu do składowej i inne. Omówię je w nieco innej kolejności niż przedstawia je powyższa tabela. 

#### 1. Operatory arytmetyczne

##### Promocje i konwersje standardowe 

Zgodnie z powyższą tabelą, w C++ wyróżnia się 13 operatorów, w tym 3 jednoargumentowe i 10 dwuargumentowych. Podstawowa zasada mówi, że jeżeli którykolwiek argument jest typu całkowitoliczbowego (np. `char` lub `bool`) i jego reprezentacja jest krótsza niż reprezentacja typu `int`, to ten argument jest *promowany* do typu `int`. Dlatego instrukcja (zwróć uwagę na `opertor +`)

```c++
std::cout << +'a' <<  "\n"; // Nie używaj tej sztuczki: preferuj zapis jawny, np. int('a')
```

nie wyświetla znaku `'a'`, lecz odpowiadającą mu liczbę typu `int` (tu: kod ASCII znaku `a`, czyli 97). Dla typu `bool` wartość `false` promowana jest do wartości 0, a `true` do wartości 1 (oczywiście w typie `int`). 

W przypadku operatorów dwuargumentowych dokonywane jest uzgodnieni ich typów tak, by typ "mniej ważny" został zamieniony na typ "bardziej ważny" bez zmiany wartości danego argumentu (o ile jest to możliwe), przy czym dla "krótkich" typów całkowitych najpierw wykonywana jest ich promocja do typu `int`. "Ważność" typów przedstawia się mniej więcej następująco: `long double` > `double`> `float`> `long long` > `long` > `int` > `short` > `signed char` > `bool`.   Jeżeli któryś z argumentów jest bez znaku (czyli posiada modyfikator `unsigned`), a drugi ma znak, to "zwycięża" typ argumentu o dłuższej reprezentacji, a jeżeli ich długości są sobie równe, to zwycięża typ bez znaku. Jeżeli oba są typami całkowitymi bez znaku o różnej długości reprezentacji bitowej, to "zwycięża" ten, który ma więcej bitów. 

Przykłady:

 ``` 
 auto i = '1' + 1   // i jest typu int i ma wartość 49, czyli wartość kodu ASCII litery '1' plus 1
 auto x = 1 + 1.0   // x jest typu double i ma wartość 2.0
 auto u = 1 - 2u    // u jest typu unsigned int i ma wartość 4294967295 (o ile unsigned reprezentowany jest na 32 bitach)
 auto z = 2 - true; // z jest typu int i ma wartość 2 - 1 = 1. Nie lubię tej konwersji. 
 auto q = -true;    // To poprawne wyrażenie o wartośći -1 w typie int, ale nie pisz tak
 ```

​      

#### Operatory jednoargumentowe `+` i `-`    

- Operator `+` nie robi nic (poza ewentualną promocją całkowitą), a operator `-` zwraca liczbę przeciwną do danej. Nigdy nie użyłem pierwszego z nich. 

```C++
int n = 1;
std::cout << +n << " " << -n << "\n"; // wyświetla 1 -1
```

-  operatory `+`, `-`, `*`, `/` mają z grubsza takie znaczenie, jak w szkole: reprezentują dodawanie, odejmowanie, mnożenie i dzielenie liczb. 