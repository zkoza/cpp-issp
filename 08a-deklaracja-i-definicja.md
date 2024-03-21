#### Definicja i deklaracja

Oto przykład *definicji* prostej funkcji w C++:

```c++
int suma(int n, int m)  // definicja (i jednocześnie deklaracja) funkcji
{
    return n + m;
}
```

Definicja funkcji składa się z typu wartości zwracanej przez funkcję (tu: `int`), nazwy funkcji (tu: `suma`), ujętej w nawiasy listy argumentów formalnych (tu: `(int n, int m)`) oraz umieszczonego w nawiasach klamrowych bloku instrukcji zwanego jej treścią (ang. *function body*, tu: `{ return n + m; }`).  Treść funkcji można pominąć, zastępując ją średnikiem. Otrzymamy w ten sposób tzw. *deklarację* (nagłówek, prototyp, interfejs) funkcji:

```c++
int suma(int n, int m);  // deklaracja funkcji
```

Przed użyciem dowolnej funkcji, kompilator musi znać jej deklarację. Definicja funkcji jest jednocześnie jej deklarację. Funkcja może mieć wiele deklaracji, ale tylko jedną definicję.  

Czasami w deklaracjach funkcji usuwa się nazwy ich parametrów. Dlatego powyższą funkcję można też zadeklarować jako

```c++
int suma(int, int);
```

Nieco bardziej złożone przykłady deklaracji funkcji (szczegóły zostaną omówione później):

```c++
void f(int & a, const std::vector<int> & v);
const int* g(const int* const p0, size_t n, size_t size);
void* h(const void* &p);
int main(int argc, const char* argv[]);
std::ostream operator<<(std::ostream & out, std::vector<int> const& v);
void X::fun(X::value_type t) const;
```



