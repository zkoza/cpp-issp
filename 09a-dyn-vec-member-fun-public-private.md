## *First things first*: funkcje składowe i sekcje *public*, *private* (i *protected*)  

Aby przejść z programowania proceduralnego ("strukturalnego") do obiektowego, zasadniczo wystarczy zamienić strukturę na klasę. Jak? Po prostu zamieniając słowo kluczowe `struct` na `class`, czyli zastępując definicję 

```c++
struct Wektor
{
    int* dane;
    size_t size;
};
```

definicją

```c++ 
class Wektor
{
    int* dane;
    size_t size;
};
```

Prawda, że proste? No nie do końca - zamiana słowa kluczowego nie oznacza przecież zmiany paradygmatu programowania, tymczasem programowanie proceduralne (ze strukturami) to zupełnie inna bestia niż programowanie obiektowe. O co więc chodzi?

Żeby zrozumieć różnicę między strukturą a klasą, spróbujmy użyć powyższej definicji klasy. Np. w następującym, poprawnym z punktu widzenia składni języka, jednak niezalecanym z punktu widzenia stylu programowani, kodzie:

```c++        
int main()
{
    Wektor w;
    w.dane = new int [5];
}
```

Gdyby `Wektor` był zadeklarowany jako `struct`, to program by się skompilował i wykonał. Jednak jeżeli użyjemy w jego deklaracji słowa kluczowego `class`, kompilator zgłosi błąd. Na przykład tak:

```txt
vector1.cpp: In function ‘int main()’:
vector1.cpp:13:7: error: ‘int* Wektor::dane’ is private within this context
   13 |     w.dane = new int [5];
      |       ^~~~
vector1.cpp:5:10: note: declared private here
    5 |     int* dane;
      |          ^~~~
```

Jak widzimy, kompilator zgłasza, że składowa wskaźnikowa `dane` jest  "prywatna" w kontekście, w jakim użyto jej w wierszu 13 i że zadeklarowano ja jako prywatną w wierszu 5 (plik `vector1.cpp`).  Aha! Czyli klasy mają coś wspólnego z prywatnością i blokadą dostępu do składowych w określonych kontekstach.

Wyjaśnienie powyższej zagadki jest dość proste: w języku C++ słowo kluczowe `class` implikuje, że na samym początku definicji klasy kompilator dopisze niewidoczną dla nas deklarację `private:`. Innymi słowy, gdy piszemy

```c++
class Wektor
{
    int* dane;
    size_t size;
};
```

kompilator widzi tu coś w rodzaju

```c++
class Wektor
{
  private:
    int* dane;
    size_t size;
};
```

Co więcej, jeśli zdefiniujemy strukturę,

```c++
struct Wektor
{
    int* dane;
    size_t size;
};
```

to kompilator C++ (ale nie C!) na jej początku wstawi deklarator `public:`

```c++
struct Wektor
{
  public:
    int* dane;
    size_t size;
};
```

W języku C nie ma klas ani deklaratorów dostępności składowych (`publ`ic, `private`, `protected`), bo nie ma w nim klas. W C++ struktur używa się znacznie rzadziej niż klas, przy czym niemal jedyną różnicą między `struct` i `class` jest to, że domyślnie w strukturach wszystko jest publiczne, a w klasach - prywatne.

Co to znaczy, że składowa jest prywatna? Najłatwiej to zrozumieć, analizując, po co wprowadzono deklaratory dostępności składowych. Otóż służą one ograniczeniu możliwości ich używania wyłącznie do twórcy danej klasy. Bardzo prosta koncepcja, której realizacja eliminuje praktycznie wszystkie kłopoty opisane w poprzednim rozdziale, jakie wiążą się ze strukturami. Oczywiście, kompilator nie wie, kto jest autorem poszczególnych fragmentów kodu, on rozpoznaje "autora" klasy w inny, bardzo prosty sposób:

- do składowych zadeklarowanych jako **prywatne** (`private`) dostęp przez `operator.`  ("notacja z kropką") lub `operator->` ("notacja ze strzałką") możliwy jest wyłącznie w kodzie znajdującym się **w przestrzeni nazw danej klasy**
- do składowych zadeklarowanych jako **chronione** (`potected`) dostęp przez `operator.`  ("notacja z kropką") lub `operator->` ("notacja ze strzałką") możliwy jest wyłącznie w kodzie znajdującym się **w przestrzeni nazw danej klasy oraz klas pochodnych** (klasy pochodne omówię przy innej okazji)
- do składowych zadeklarowanych jako **publiczne** (`public`) dostęp przez `operator.`  ("notacja z kropką") lub `operator->` ("notacja ze strzałką") możliwy jest **wszędzie**, tak jak dla składowych klasycznych struktur.

(uwaga: w powyższej wyliczance brakuje jeszcze funkcji zaprzyjaźnionych, ale to nieco inna bajka).

No dobrze, a gdzie znajduje się przestrzeń nazw danej klasy? Powoli do tego dojdziemy. W naszym przykładzie:

```c++
class Wektor
{
  private:
    int* dane;
    size_t size;
};
```

ona rozciąga się od klamry `{` otwierającą definicję składowych klasy do klamry `}` ją zamykającej.

Jeśli domyślnie wszystko w klasach jest zablokowane deklaratorem `private:`, to w jaki sposób używać obiektów danej klasy poza jej definicją? Pamiętamy przy tym, że struktur najwygodniej używa się poprzez specjalnie w tym celu napisane funkcje. Zwyczajne funkcje znajdują się jednak poza przestrzenią nazw danej klasy i nie powinny mieć dostępu do jej składowych. Wnioskujemy stąd, że dobrze byłoby móc umieszczać funkcje w przestrzeni nazw klasy i w dodatku oznaczać je jako publiczne. I rzeczywiście, istnieje specjalna składnia realizująca tę koncepcję. Oto przykład, który chyba wszystko wyjaśni:

```c++
#include <iostream>
class Wektor
{
    int* dane;    // domyślnie: składowa prywatna
    size_t size;  // domyślnie: składowa prywatna
  public:
    auto get_size() { return size; } // tu można używać składowych prywatnych 
    auto set_size(size_t new_size) { size = new_size; } // tu także
};

int main()
{
    Wektor w;
    w.set_size(6);  // użycie publicznej funkcji składowej 
    std::cout << w.get_size() << "\n"; // użycie publicznej funkcji składowej 
}
```

Jak widać, w definicji klasy można umieszczać definicje funkcji (tu: `get_size` o `set_size`). Każda funkcja, tak jak każda składowa z danymi, należy do sekcji publicznej, chronionej lub prywatnej. Dostęp do nich (czyli sposób wywoływania) jest taki, jak do składowych z danymi: poprzez kropkę (np. `w.set_size(6)`) lub strzałkę (np. `p->set_size(6)`, jeżeli `p` jest wskaźnikiem na `Wektor`). W ten sposób twórca klasy może zdefiniować dane, jakie posiada każdy z jej obiektów, a także, po zablokowaniu zewnętrznego dostępu do tych danych deklaratorem `private`, kompletny interfejs tego obiektu w postaci zbioru publicznych funkcji składowych. Zwykły użytkownik nie może rozszerzyć zbioru funkcji działających na danym obiekcie, bo nie ma dostępu do jego danych. Genialne w swojej prostocie! Co to daje? Daje to możliwość tworzenia nawet bardzo złożonych klas obiektów ze ściśle określonym interfejsem (=zbiorem dopuszczalnych operacji), którego elementy (=funkcje składowe) zawsze działają w przewidywalny, poprawny i efektywny sposób. Twórca biblioteki zostaje odseparowany od jej użytkowników. Tworzenie poprawnego, czytelnego, a jednocześnie wciąż bardzo efektywnego kodu ulega ogromnemu uproszczeniu.

Reasumując, powoli powinno zacząć układać się nam w głowach, w czym leży główna różnica między strukturą a obiektem

- struktura to po prostu zbiór niepowiązanych ze sobą danych
- obiekt (klasy) to zbiór ściśle ze sobą powiązanych danych ORAZ kompletu wszystkich dopuszczalnych operacji na tych danych.  

W programowaniu proceduralnym mamy dane i zupełnie od nich niezależne funkcje ("procedury"), które te dane przetwarzają. W programowaniu obiektowym dane zostają ściśle zintegrowane z operacjami, które można na nich wykonać. Obiekt to już nie tylko fragment pamięci operacyjnej, ale i zbiór wszystkich dopuszczalnych elementarnych operacji na tej pamięci. Użytkownik może tworzyć własne operacje tylko jako kombinację operacji, które udostępnia mu interfejs (publiczny, chroniony) klasy tego obiektu.   