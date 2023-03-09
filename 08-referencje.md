# Referencje

Referencja to nowa nazwa dla już istniejącej zmiennej czy obiektu.

```c++
int n = 0;
int & ref = n;
std::cout << n << "\n";  // wyświetli 0
ref = 1;                 // równoważne instrukcji n = 1;
std::cout << n << "\n";  // wyświetli 1
```

 Nie ma w tym wielkiej filozofii, prawda? Standard nic nie mów o tym, jak implementowane są referencje w C++. Popatrzmy więc na prosty przykład:

```c++
int f(int num) 
{
   int & n = num; 
   return n;
}

int g(int num) 
{
   int* n = &num; 
   return *n;
}
```

Mamy tu dwie funkcje, przy czym w pierwszej posłużono się referencją, a w drugiej - wskaźnikiem. Obie jednak robią dokładnie to samo. Jak obie funkcje skompiluje kompilator? Na przykład tak (kod assemblera zaczerpnięty z [Compiler Explorer](https://godbolt.org/) dla kompilatora gcc 12.2):

```assembly
f(int):
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-20], edi
        lea     rax, [rbp-20]
        mov     QWORD PTR [rbp-8], rax
        mov     rax, QWORD PTR [rbp-8]
        mov     eax, DWORD PTR [rax]
        pop     rbp
        ret
g(int):
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-20], edi
        lea     rax, [rbp-20]
        mov     QWORD PTR [rbp-8], rax
        mov     rax, QWORD PTR [rbp-8]
        mov     eax, DWORD PTR [rax]
        pop     rbp
        ret
```

Nie musimy znać asemblera by zauważyć, że kompilator obie wersje kodu przetłumaczył w identyczny sposób. Referencja w C++ to "ukryty wskaźnik". Innymi słowy, instrukcja

```c++
int & n = num;
```

może spowodować utworzenie w pamięci nowej zmiennej o nazwie `n`, w której zostanie zapisany adres zmiennej num. Referencje nie wymagają używania notacji wskaźnikowej: ani deklaracja takiej zmiennej, ani jej użycie nie wymaga użycia gwiazdki. Ich składnia jest dokładnie taka, jak w przypadku zmiennej lub obiektu, z którym są związane.  

Inicjalizacja zmiennej wskaźnikowej nie wymaga stosowania operatora pobrania adresu (`&`).

Referencje muszą być zainicjalizowane. Nie można utworzyć referencji do nie wiadomo czego:

```c++
int & n;   // Błąd: referencja n wymaga inicjalizatora
```

Oprócz zwykłych referencji istnieją też referencje stałe:

```c++
const int & m = num;
```

Odpowiadają one wskaźnikom na stałą, tzn. nie można za ich pomocą modyfikować zmiennej czy obiektu, z którym są związane:

```c++
int k = 0;
const int & m = k;      // m jest stałą referencją do k
std::cout << m << "\n"; // OK
m = 1;                  // Błąd: m jest stałą referencją do k
```

Mimo że dla kompilatora referencja to "ukryty wskaźnik", dla programisty jest to inna nazwa istniejącego obiektu. 

```c++
char c = 'a';
char & n = c;
std::cout << sizeof(n) << "\n";  // wyświetli 1, czyli sizeof(k), czyli rozmiar c a nie rozmiar n
```

 