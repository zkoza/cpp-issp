## Argumenty domyślne funkcji

Zdarza się, że funkcje mogą mieć argumenty, których zwyczajny użytkownik nie chciałby podawać. Na przykład jeżeli w bibliotece Qt chcemy wybrać czcionkę, to używamy klasy `QFont`, która ma następujący konstruktor: 

```c++
QFont::QFont(const QString &family, int pointSize = -1, int weight = -1, bool italic = false)
```

Jak widać, mamy tu funkcję z czterem argumentami, przy czym dla 3 ostatnich podano tzw. parametry domyślne (tu: `-1`, `-1` i `false`). Argument pierwszy nie ma wartości domyślnej, więc zawsze musi być podany w sposób jawny. Przy takiej deklaracji, obiekt reprezentujący czcionkę można utworzyć na kilka sposobów:

```c++
QFont font0("Arial");             // równoważne: QFont font0("Arial, -1, -1, false);
QFont font1("Arial", 20);              // czyli: QFont font1("Arial, 20, -1, false);
QFont font2("Arial", 20, QFont::Bold); // czyli: QFont font2("Arial, 20, 700, false);
QFont font3("Arial", 20, QFont::Bold, true); // czyli: QFont font3("Arial, 20, 700, true);
```

W powyższym przykładzie 

- `font0` reprezentuje czcionkę z rodziny Arial o wysokości zdefiniowanej przez system operacyjny (zwykle 12 punktów), domyślnej grubości linii i w kształcie prostym.
- `font1` reprezentuje czcionkę z rodziny Arial o wysokości 20 punktów (pikseli), domyślnej grubości linii i w kształcie prostym.
- `font1` reprezentuje czcionkę z rodziny Arial o wysokości 20 punktów, pogrubioną i w kształcie prostym.
- `font1` reprezentuje czcionkę z rodziny Arial o wysokości 20 punktów, **pogubioną** i *pochyloną*.

W większości przypadków zupełnie wystarcza nam czcionka prosta i niepogrubiona. Nie chcemy też tracić czasu na wyszukiwanie informacji o tym, w jaki sposób przekazuje się do tej funkcji informację o grubości czcionki (tu: `QFont::Bold`). Dlatego najczęściej będziemy używać wywołań z 2 argumentami, pomijając argument 3. i 4. Często zaś pominiemy nawet argument drugi.

W argumentach domyślanych nie ma wielkiej filozofii, jest wygoda. Te argumenty, których nie podamy w sposób jawny przy wywołaniu funkcji zostaną za nas dopisane przez kompilator. W powyższym przykładzie każdy z obiektów `font0`, ..., `font3` jest inicjalizowany funkcją 4-argumentową. 

Argumenty domyślne są zawsze ustawiane na końcu listy argumentów formalnych funkcji. 

Innym przykładem funkcji z argumentami domyślnymi jest funkcja konwertująca napis na liczbę, `std::stoi`. Jej deklaracja wygląda następująco:

```c++
int stoi( const std::string& str, std::size_t* pos = nullptr, int base = 10 );
```

Większości użytkowników nie interesuje jednak to, że można ją wywołać z 3 argumentami, zwykle używa się jej z jednym:

```c++
int n = std::stoi("123");
```

Jeżeli drugi argument ma wartość różną od `nullptr`, to funkcja zapisze w nim liczbę przetworzonych znaków w argumencie `str`. Z kolei trzeci argument pozwala określić podstawę systemu liczbowego, w którym zapisano liczbę. Np. w ten sposób można zamienić liczbę szesnastkową na dziesiętną:

```c++
int n = std::stoi("abc", nullptr, 16);
```

 Chodzi jednak o to, że w ponad 99% przypadków użytkownik nie chce używać 2. i 3. argumentu tej funkcji ani, tym bardziej, wczytywać się w jej dokumentację. 