## Implementacja wektora jako struktury 

Implementację tablicy dynamicznej (w żargonie C++ zwanej "wektorem") rozpoczniemy od krótkiego wypadu w stronę języka "C" i przedstawienia (fragmentu) jej implementacji za pomocą narzędzi w nim dostępnych. Czyli bez klas.

Skoro tablica dynamiczna to struktura danych funkcjonalnie przypominająca tablicę (np. tą zdefiniowaną instrukcją `int tab[10];`), nie ograniczona jednak żądaniem, by jej rozmiar był znany w czasie kompilacji, to jedynym miejscem, w którym w rozsądny sposób można przechowywać dane zapisywane w tej tablicy, jest sterta programu (przypominam, że stos jest zarządzany w całości przez kompilator, nie nadaje się więc do implementacji dynamicznych struktur danych). Przypominam, że pamięć na stercie alokowana jest operatorem `new`, nie ma własnej nazwy, a dostęp do niej zapewniany jest przez wskaźnik. Stąd dość naturalna, minimalna definicja struktury danych reprezentujących wektor dynamiczny:

```c++       
struct Wektor
{
    int* dane;   // wskaźnik do tablicy z danymi
    size_t size; // liczba elementów tej tablicy
};
```

Jak widzimy, nasz `Wektor` składa się ze składowej wskaźnikowej (co rozpoznajemy po gwiazdce w jego definicji) `dane` oraz składowej liczbowej `size` przechowującej aktualny rozmiar wektora. Składowa `dane` wskazuje początek tablicy z danymi przechowywanymi w wektorze, a składowa `size` przechowuje informację o ich liczbie.
Co można zrobić z taką strukturą danych, pamiętając, że ma ona reprezentować tablicę dynamiczną? Można nią operować ręcznie

```  c++
Wektor w;
w.size = 5;
w.dane = new int[5];
w.dane[0] = 8;
```

ale nie ma to większego sensu. Równie dobrze moglibyśmy posługiwać się dwoma niezależnymi zmiennymi, `dane` i `size	` - przynajmniej byłoby mniej pisania. Przygotujmy więc sobie kilka funkcji operujących na strukturach typu `Wektor`. Najważniejsze są funkcje inicjalizujące wektor, zwalniające przydzieloną mu pamięć po zakończeniu pracy z nim oraz funkcje umożliwiające odczytywanie i zapisywane danych. Mogą one wyglądać następująco:

```c++ 
void init(Wektor& v, size_t size, int n0 = 0)
{
    v.dane = new int[size];
    v.size = size;
    for (size_t i = 0; i < size; i++)
    {
        v.dane[i] = n0;  // n0 to wartość domyślna każdego nowego Wektor-a
    }
}

void free(Wektor& v)
{
    delete[] v.dane;
    v.dane = nullptr;
    v.size = 0;
}

int get_value(Wektor& v, size_t index) 
{ 
    return v.dane[index]; 
}

void set_value(const Wektor& v, size_t index, int value) 
{ 
    v.dane[index] = value; 
}
```

Można ich użyć np. w następujący sposób:

```c++
int main()
{
    Wektor w;
    init(w, 10);
    set_value(w, 5, 2);
    set_value(w, 3, get_value(w, 5) * 4);
    free(w);
}
```

Czy powyższy kod jest poprawny? Tak. Czy jest czytelny? Nie za bardzo. Czy jest wystarczająco szybki? To zależy od kompilatora, ale tak, jest bardzo szybki. Czy powyższy styl programowania jest wygodny dla twórców bibliotek? Cóż, w ten sposób pisze się biblioteki w języku C, więc da się z tym żyć. Niemniej, jest w tym stylu sporo niewygody. Użytkownik może w swoim kodzie dowolnie używać informacji o wewnętrznej strukturze (budowie) każdego obiektu typu `Wektor`. Może więc robić takie "sztuczki":

```c++ 
Wektor w;
// 1
w.size = 0;
w.dane = nullptr;
// 2
Wektor v;
v[6] = 8;
// 3
w.dane = new int [10];
w.size = 20;
// 4
int k;
w.dane = &k;
// 5
int z = 4*sizeof(w.size);
```

W pierwszym fragmencie programista tworzy `Wektor`, po czym od razu go "zeruje". To wydaje się poprawne, prawda? A co, jeśli jakaś biblioteka będzie funkcjonować przez 20 lat, po czym ktoś dojdzie do wniosku, że musi do jakiejś struktury danych dodać jeszcze jedną składową? Nasz kod pominie jej inicjalizację, będziemy mieli obiekt ze śmieciem, więc nie można tego zrobić. Twórca biblioteki zapewne zostawi starą strukturę w spokoju i dostarczy jej nową wersję, z nową nazwą (Microsoft w takim przypadku dodawał kiedyś do pierwotnej nazwy struktury literki `Ex`, co nieco wyjaśnia, skąd wzięły się funkcje o nazwach w rodzaju `CreateWindowEx`).

W drugim fragmencie programista po prostu zapominał zainicjalizować wektor i od razu próbuje go użyć (`v[6] = 8`). To się prawdopodobnie skończy katastrofą.

W trzecim fragmencie wektor utracił wewnętrzną spójność danych: składowa `dane` ma nieprawidłową wartość niezwiązaną z faktyczną liczbą elementów tablicy.

W czwartym fragmencie programista jako bufora danych chce użyć już istniejącej tablicy. To byłby fantastyczny pomysł gdyby nie to, że jeżeli za chwilę wywoła na swoim obiekcie funkcję `free`, to program zapewne padnie, gdyż operator `delete[]` można stosować tylko na adresach zwróconych przez operator `new[]`.

W piątym przykładzie programista robi zapewne jakieś założenia co do typu składowej  `size`. A co, jeśli twórca biblioteki zmieni kiedyś jej typ?

Powtórzę: można w powyższy sposób programować, tak działa język C, a tak wygląda rzeczywisty program wykorzystujący napisaną w tym stylu bibliotekę [GSL](https://www.gnu.org/software/gsl/doc/html/vectors.html):

```c
int main (void)
{
  int i;
  gsl_vector * v = gsl_vector_alloc (3);

  for (i = 0; i < 3; i++)
    {
      gsl_vector_set (v, i, 1.23 + i);
    }

  for (i = 0; i < 100; i++) /* OUT OF RANGE ERROR */
    {
      printf ("v_%d = %g\n", i, gsl_vector_get (v, i));
    }

  gsl_vector_free (v);
  return 0;
}
```

Dodam, że niektóre firmy, w tym Microsoft, żeby wyeliminować część z powyższych problemów, nie udostępnia definicji używanych przez siebie struktur. Część z ich bibliotek zawiera wyłącznie funkcje, których my nie możemy już zmienić czy rozszerzyć. Dlaczego? Bo ich argumentem, zamiast `Wektor`, jest wskaźnik na `void`. Czyli tak, jakby zamiast funkcji o sygnaturze

```
void init(Wektor& v, size_t size, int n0 = 0);
```

programista miał do dyspozycji funkcję o sygnaturze

```c++
void init(void* v, size_t size, int n0 = 0);
```

lub coś równie niezdrowego, gdzie nie wiadomo, jakiego typu tak naprawdę jest pierwszy argument.

W obiektowym paradygmacie programowania powyższe problemy rozwiązuje się zupełnie innymi metodami.
