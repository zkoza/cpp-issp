## Operator przypisania i pseudowskaźnik `this`

Na pewno chcielibyśmy móc przypisywać wartość jednego wektora drugiemu, np. w takim kontekście:

```c++
Wektor v(4);
Wektor w(6);
// rób coś z w i v
v = w;
```

Jak to zrobić? Odpowiedź będzie niespodzianką: nie musisz robić niczego

- kompilator sam za ciebie wygeneruje kod dla tej instrukcji

- ten kod w tym przypadku będzie błędny

- jeżeli uruchomisz powyższy kod, to program padnie, prawdopodobnie z komunikatem 

  ```txt
  free(): double free detected in tcache 2
  ```

  który, przy pewnym doświadczeniu, odczytujemy jak "ponowne wywołanie operatora `delete[]` na  pamięci, która już nim została zwolniona". 

Tyle pracy, tyle funkcji składowych, a nasz program po prostu jest błędny! Jego źródłem jest podjęta dawno temu decyzja o tym, by programy w C++ były możliwie zgodne z programami w języku C, w którym nie ma jednak destruktorów i tam taki automatycznie generowany kod działa, a w C++ już nie (jeżeli twoja klasa potrzebuje destruktora, to `operator=` generowany przez kompilator niemal na pewno wysadzi twój program w powietrze).  Teraz nieco lepiej rozumiesz, dlaczego C++ uchodzi za język trudny i dlaczego ma mnóstwo zarówno zwolenników, jak i przeciwników.

Co robić? Oczywiście należy zdefiniować własny operator przypisania (czyli `operator=`) w klasie `Wektor`! Poniżej wersja może niezbyt efektywna, ale działająca.   

```c++
Wektor& operator=(const Wektor& rhs)
{
    if (this == &rhs) return *this;  //  nie ma nic do roboty...

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
```

Zacznijmy od instrukcji

```c++
if (this == &rhs) return *this;
```

Występuje w nim tajemniczy identyfikator `this`. W języku C++ jest to słowo kluczowe o znaczeniu podobnym do `self` w Pythonie, tyle że w języku Python nie ma wskaźników, `self` jest więc tam referencją, a w dodatku `self` nie jest tam też słowem kluczowym, więc nazwa `self` to tylko konwencja. "This" jest na tyle ważne w programowaniu obiektowym, że poświęcono mu całkiem pokaźny wpis w [Wikipedii ](https://en.wikipedia.org/wiki/This_(computer_programming)). W C++ reprezentuje adres obiektu, na którym w danej chwili wywoływana jest dana funkcja składowa. 

Rozpatrzmy na chwilę następujący przykład:

```c++
class X
{
  public:
    auto This() const { return this; }
};

int main()
{
    X obj1;
    X obj2;
    std::cout << &obj1 << " " << &obj2 << "\n";
    std::cout << obj1.This() << " " << obj2.This() << "\n";
}
```

Mamy tu klasę `X` z jedną publiczną funkcją składową `This`, która zwraca wartość `this` (dzięki zapisaniu typu wartości nie muszę teraz wiedzieć, jaki jest typ tego `this` ). W funkcji `main` tworzę dwa obiekty tej klasy, następnie wyświetlam ich adresy pobrane operatorem pobrania adresu, `&`.  Jeśli teraz wyświetlę wartości zwracane przez składową `This`, otrzymam dokładnie te same wartości, np. takie:

```txt
0x7ffeae6c05de 0x7ffeae6c05df
0x7ffeae6c05de 0x7ffeae6c05df
```

Jeżeli funkcja `This` jest wywoływana na obiekcie `obj1`, to zwraca ona adres `obj1`, czyli `this` w jej implementacji reprezentuje adres `obj1`. Jeśli zaś wywołam ją na obiekcie `obj2`, to `This` zwróci adres `obj2`, czyli `this` w kodzie funkcji `This` reprezentować będzie adres `obj2`. Ogólnie: `this` może pojawić się wyłącznie w kodzie funkcji składowych klas, i to tylko tych funkcji, które wywołuje się na obiektach (póki co innych nie widzieliśmy, ale takowe istnieją) i reprezentuje adres obiektu, na którym jest wywoływana. 

To samo inaczej. Jeśli mamy dwa wektory `v`i `w`, i odczytujemy ich rozmiar:

```c++
a = v.size();
b = w.size();
```

to spodziewamy się, że jeżeli te wektory mają różne rozmiary, to `size` za każdym razem zwróci inną, prawidłową wartość. Ta funkcja musi więc mieć dostęp do obiektu, który w "notacji z kropką" występuje przed kropką. Ten dostęp zapewnia właśnie `this`, który można traktować jak wskaźnik na ten obiekt.   

Możemy więc w powyższym kodzie zastąpić `auto` prawidłowym typem funkcji `This` i nawet zrobić to dla wersji `const` i nie-`const`:

```c++
class X
{
  public:
    const X* This() const { return this; }
    X* This()  { return this; }
};
```

I tu mamy pierwszą zagadkę rozwiązaną: w funkcjach składowych stałych `this` jest wskaźnikiem na stały obiekt  (tu: `const X*`), a w funkcjach składowych bez `const` jest on zwykłym wskaźnikiem (tu: `X*`). Wskaźnik na stałą chroni wskazywany przez siebie obiekt prze modyfikacją, dlatego funkcje składowe z modyfikatorem `const` nie mogą modyfikować stanu obiektu, na którym są wywoływane, choć mogą ten stan odczytywać. 

Wracamy do klasy `Wektor` i operatora przypisania. Teraz rozumiemy, że instrukcja warunkowa `if` w tym fragmencie kodu

```c++ 
Wektor& operator=(const Wektor& rhs)
{
    if (this == &rhs) return *this;  //  nie ma nic do roboty...
```

sprawdza, czy funkcji `operator=` nie wywołano na obiekcie `rhs`, czyli prawym argumencie `opeartor=`.  Innymi słowy, czy nie wykonujemy operacji w rodzaju

```c++
Wektor v;
v = v;
```

Ta operacja jest bowiem trywialna: przypisanie obiektu na samego siebie nie powinno zmienia stanu tego obiektu, czyli być operacją pustą. 

W dalszej części implementacji funkcji `operator=` mamy sprawdzenie, czy stary rozmiar zgadza się z nowym. Jeśli nie, to zwalniamy pamięć przydzieloną na starą tablicę i alokujemy nową:

```c++ 
if (size() != rhs.size())    
{        
    delete[] _dane;
    _dane = new int[rhs.size()];    
}
```

Teraz przepisujemy dane z obiektu stojącego po prawej stronie (ang. *right-hand side*, stąd jego nazwa `rhs`) operatora przypisania:

```c++
for (size_t i = 0; i < rhs.size(); i++)
{
    _dane[i] = rhs[i];
}

```

Pozostaje nam zaktualizować rozmiar wektora:

```c++
_size = rhs.size();
```

i zwrócić wartość funkcji, czyli referencję do jej lewego argumentu, który, jak wiemy, jest wskazywany przez `this`:

```c++ 
return *this;
```

### Inne zastosowania `this`

Spójrzmy na ten fragment kodu:

```c++
Wektor& operator=(const Wektor& rhs)
{
    if (this == &rhs) return *this;  //  nie ma nic do roboty...

    if (size() != rhs.size())
    {
        delete[] _dane;
        _dane = new int[rhs.size()];
    }
```

Skoro `_dane` są składową obiektu, na którym wywoływana jest powyższa funkcja, to można w powyższym kodzie napisać `thuis->_dane` i efekt będzie ten sam. Podobnie jest z funkcją składową `size`. Dlatego powyższy kod można by zapisać tak:



```c++
Wektor& operator=(const Wektor& rhs)
{
    if (this == &rhs) return *this;  //  nie ma nic do roboty...

    if (this->size() != rhs.size())
    {
        delete[] this->_dane;
        this->_dane = new int[rhs.size()];
    }
```

Niektórzy uważają taki zapis za bardziej czytelny, większość jednak opuszcza `this->` w powyższym kontekście. 

Spójrzmy jeszcze na ten fragment:

```c++
    for (size_t i = 0; i < rhs.size(); i++)    
    {        
        _dane[i] = rhs[i];    
    }
```

Tutaj `_dane` jest wskaźnikiem, więc `_dane[i]` to odwołanie bezpośrednie do elementu tablicy `_dane`. Jak w tym kontekście wywołać na lewym argumencie naszej funkcji `operator[]`? Odpowiedź:



```c++
    for (size_t i = 0; i < rhs.size(); i++)    
    {        
        (*this)[i] = rhs[i];    
    }
```

lub nawet

```c++
    for (size_t i = 0; i < rhs.size(); i++)    
    {        
        this->operator[](i) = rhs[i];    
    }
```

#### `this` w C++ a `self` w Pythonie

Aby lepiej zrozumieć `this`, warto porównać go z jego odpowiednikami w innych językach programowania, np. z `self` w Pythonie. W serwisie [Geegs for geeks](https://www.geeksforgeeks.org/self-in-python-class/) znalazłem taki opis:

*When working with classes in Python, the term “self”  refers to the instance of the class that is currently being used. It is  customary to use “self” as the first parameter in instance methods of a  class. Whenever you call a method of an object created from a [class](https://www.geeksforgeeks.org/python-classes-and-objects/), the object is automatically passed as the first argument using the  “self” parameter. This enables you to modify the object’s properties and execute tasks unique to that particular instance.*

Ten opis jest ciekawy także ze względu na nomenklaturę. W informatyce często te same lub bardzo podobne koncepcje w różnych środowiskach opisywane są nieco innym językiem. Tutaj *instance of a class* to po prostu "obiekt danej klasy", *instance method* to "funkcja składowa klasy operująca na obiektach tej klasy", natomiast *an object created from a class* to "obiekt klasy". Powyższemu opisowi twarzyszy kod przykładowy:

```python
class mynumber:
    def __init__(self, value):
        self.value = value
     
    def print_value(self):
        print(self.value)
 
obj1 = mynumber(17)
obj1.print_value()
```

Widzimy, że w języku Python funkcje składowe klas, które są wywoływane na obiektach (tzw. "metody"), mają wyróżniony pierwszy parametr, zwyczajowo nazywany `self`. Parametr ten występuje w ich definicji, ale nie używa się go podczas wywoływania takich funkcji (byłoby to bardzo niewygodne, gdyż wymagałoby to powtórzenia argumentu stojącego przed kropką, np. `ob1.print_value(obj)`). Porównajmy początek definicji, `print_value(self)` i wywołanie: `obj1.print_value()` (0 zamiast 1 jawnego argumentu). W Pythonie ten pierwszy argument jest uzupełniany automatycznie przez interpreter. Co to ma wspólnego z C++? Otóż w C++ jest dokładnie tak samo, tyle że skoro ten pierwszy argument jest używany zawsze i łato daje się to zautomatyzować, to nie tylko nie musimy, ale wręcz nie możemy deklarować go wraz z pozostałymi argumentami funkcji składowej! (Notabene, przypomina to nieco zakaz podawania `void` jako "wartości" konstruktora czy destruktora: jeśli coś w programie ma stałą postać, to niech zajmie się tym kompilator). W przeciwieństwie do Pythona, w C++ nie deklarujemy tego wyróżnionego argumentu. On zawsze nosi nazwę `this` i zawsze jest obsługiwany automatycznie przez kompilator. Czyli troszkę tak, jakbyśmy pisali 

```c++     
class X
{
  public:
    int kwadrat(int n) const { return n * n; }
};
```

a kompilator zamieniał to na coś w rodzaju

```c++
int X::kwadrat(const X *this, int n) { return n * n; }
```

Podobnie wywołanie funkcji poprzez "notację z kropką",  np. 

```c++ 
X x;
x.kwadrat(7);
```

można zamienić na wywołanie "zwyczajnej funkcji"

```c++ 
X x;
X::kwadrat(&x, 7);
```

Prawdę mówiąc, pierwsze kompilatory C++ działały dokładnie w ten sposób: zamieniały kod źródłowy języka C++ na kod źródłowy języka C. W szczególności, w stosunku do (niestatycznych) funkcji składowych klas dokonywały transformacji właśnie takiej jak powyższa.  Warto przy okazji zauważyć, co w tej interpretacji oznacza `const` w deklaracji stałej funkcji składowej, np. `int kwadrat(int n) const`. Jest to modyfikator modyfikujący typ `this`. Innymi słowy, po transformacji do języka C,  on modyfikuje typ wskazywany przez ten `this`. 

Na koniec: dlaczego `this` w C++ jest pseudowskaźnikiem a nie wskaźnikiem? Gdyż nie jest zwykłą zmienną, a raczej wartością. W szczególności, nie można pobrać jego adresu.

### Podsumowanie

- Jeżeli Twoja klasa potrzebuje destruktora (lub konstruktora kopiującego), to niemal na pewno potrzebuje też operatora przypisania. 
  - Jeżeli wśród składowej Twojej klasy są "nagie" wskaźniki, to niemal na pewno potrzebuje ona własnego operatora przypisania (oraz konstruktora kopiującego i destruktora).
- `this` jest (pseudo)wskaźnikiem używanym wyłącznie w treści funkcji składowych klas i wskazującym na obiekt, na którym dana funkcja jest w tej chwili wywoływana. 
  - można go w myślach traktować jako pierwszy ("zerowy"), niejawny argument każdej takiej funkcji  