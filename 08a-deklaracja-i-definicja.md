#### Definicja i deklaracja

Oto przykład *definicji* prostej funkcji w C++:

```c++
int suma(int n, int m)
{
    return n + m;
}
```

Definicja funkcji składa się z typu wartości zwracanej przez funkcję (tu: `int`), nazwy funkcji (tu: `suma`), ujętej w nawiasy listy argumentów formalnych (tu: `(int n, int m)`) oraz umieszczonego w nawiasach klamrowych bloku instrukcji zwanego jej treścią (tu: `{ return n + m; }`).  Treść funkcji można pominąć, zastępując ją średnikiem. Otrzymamy w ten sposób tzw, deklarację (nagłówek) funkcji:

```c++
int suma(int n, int m);
```

Przed użyciem dowolnej funkcji, kompilator musi znać jej deklarację. 