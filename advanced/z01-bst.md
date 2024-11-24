### Binarne drzewo poszukiwań 

Binarne drzewo poszukiwań (ang. *Binary Search Tree*, BST) to dynamiczna struktura danych, w której każdy element, zwany w tym kontekście węzłem, posiada trzy wskaźniki, zwyczajowo zwane lewym i prawym potomkiem oraz rodzicem, a także co najmniej jedną składową przechowującą dane, zwaną kluczem (ang. *key*):

```c++
struct Node
{
    Node* left;
    Node* right;
    Node* parent;
    int key; 
};
```

Często oprócz samego klucza spotyka się w takiej strukturze także składową `value`, co stanowi podstawę do konstrukcji słowników, w których kluczom przyporządkowuje się wartości,  tu jednak nie będę wchodził w także szczegóły techniczne. Typ klucza może być niemal dowolny, wymagamy tylko, by klucze można było  porządkować, np. operatorem `operator<`.     

Dwa z trzech wskaźników zawartych w każdym węźle BST, `left` i `right`, pozwalają tworzyć [drzewo binarne](https://pl.wikipedia.org/wiki/Drzewo_binarne), czyli drzewo, w którym każdy węzeł ma co najwyżej 2 potomków. Trzeci wskaźnik, `parent`, prowadzi do węzła nadrzędnego (rodzica) i umożliwia implementację pewnych algorytmów związanych z wyszukiwaniem w drzewie kluczy o określonych cechach (np. gdzie się znajduje i jaką ma wartość najmniejszy spośród kluczy większych od danego klucza). [Binarne drzewo poszukiwań](https://pl.wikipedia.org/wiki/Binarne_drzewo_poszukiwa%C5%84) to z kolei takie drzewo binarne, w którym lewe poddrzewo każdego węzła zawiera wyłącznie elementy o  kluczach mniejszych niż klucz tego węzła, a prawe poddrzewo zawiera wyłącznie elementy o kluczach nie mniejszych niż klucz tego węzła. [Przykład](https://pl.wikipedia.org/wiki/Binarne_drzewo_poszukiwa%C5%84) drzewa BST (na którym pominięto wskaźniki `parent`):

<img src="./img/z01/Binary_search_tree.png" style="zoom:50%;" />

W każdym drzewie binarnym jeden węzeł jest wyróżniony: nie ma on rodzica. Nazywamy go korzeniem drzewa. Odpowiadającej mu składowej `parent` przypisujemy w C++ wartość `nullptr`. 

Większość funkcji związanych z drzewami BST najwygodniej pisze się za pomocą rekurencji. Tak może wyglądać napisana w ten sposób funkcja `insert`:

```c++
Node* insert(Node*& root, int n)
{
    if (root == nullptr)
    {
        root = new Node{.left = nullptr, .right = nullptr, .parent = nullptr, .key = n};
        return root;
    }
    if (n < root->key)
    {
        auto tmp = insert(root->left, n);
        if (tmp == root->left)
            root->left->parent = root;
        return root->left;
    }
    auto tmp = insert(root->right, n);
    if (tmp == root->right)
        root->right->parent = root;
    return root->right;    
}
```

Funkcja ta zwraca wskaźnik do nowo wstawionego węzła, co często okazuje się bardzo użyteczną informacją. Implementacja składa się z trzech oddzielnych części. 

- Pierwsza instrukcja `if` obsługuje przypadek wstawiania nowego węzła do pustego drzewa. 
- Drugi `if` w gałęzi `true` obsługuje sytuację, gdy nowy klucz ma wartość mniejszą od klucza znajdującego się w aktualnym korzeniu (pod)drzewa. Warto bowiem zauważyć, że każde poddrzewo drzewa BST też jest drzewem BST, dlatego argumentem funkcji `insert` może być wskaźnik na dowolny węzeł tego drzewa, nie tylko ten na jego "prawdziwy" korzeń. Funkcja ta umieszcza klucz we właściwym miejscu rekurencyjnie. Dodatkowo, w instrukcji `if`, prawidłowo aktualizuje ona wartość składowej `parent`  nowego węzła. 
- Gałąź `else` drugiej instrukcji `if` obsługuje przypadek, w którym wstawiany klucz jest większy lub równy kluczowi w bieżącym korzeniu poddrzewa. Implementacja jest lustrzanym odbiciem poprzedniego przypadku.   

 #### Zwalnianie pamięci - funkcja `free`

Rekurencyjna implementacja funkcji zwalniającej pamięć zajmowaną przez drzewo BST jest dość prosta i bardzo przypomina swój odpowiednik dla list pojedynczo i podwójnie wiązanych:

```c++
void free(Node*& root)
{
    if (root == nullptr)
        return;
    free(root->left);
    free(root->right);
    delete root;
    root = nullptr;
}
```

#### Weryfikacja niezmienników drzewa

Poprawnie skonstruowane drzewo BST powinno spełniać pewne warunki. Na przykład jeżeli pewien węzeł posiada lewego potomka, to musi być w tym potomku oznaczony jako jego rodzic, a jego klucz musi być mniejszy od klucza rodzica. Warunki te sprawdza poniższa funkcja: 

```c++
void verify(const Node* root, bool root_node)
{
    if (root == nullptr)
        return;
    if (root->left != nullptr && root->left->parent != root)
        throw std::logic_error("Invalid BST tree structure (left branch)");
    if (root->right != nullptr && root->right->parent != root)
        throw std::logic_error("Invalid BST tree structure (right branch)");
    if (root_node && root->parent != nullptr)
        throw std::logic_error("Invalid BST tree structure (parent)");
    if (root->left != nullptr && root->left->key >= root->key)
        throw std::logic_error("Invalid BST tree structure (left key comparison)");
    if (root->right != nullptr && root->right->key < root->key)
        throw std::logic_error("Invalid BST tree structure (right key comparison)");

    verify(root->left, false);
    verify(root->right, false);
}
```

##### Dygresja: shortcut evaluation

Dygresja. Operatory `&&` i `||` w języku C++ posiadają pewną ważną własność, która czasami znacznie upraszcza kod, z języka angielskiego zwaną ***shortcut evaluation*** bądź ***[short-circuit evaluation](https://en.wikipedia.org/wiki/Short-circuit_evaluation)***, czyli "wartościowanie skrócone" lub "wartościowanie po najkrótszej ścieżce". Polega ona na tym, że w wyrażeniu `a && b` (lub `a || b`), w którym `a` i `b` to dowolnie złożone wyrażenia, najpierw wyznaczana jest wartość `a` i jeżeli na tej podstawie można przewidzieć wartość całego wyrażenia `a && b` (lub `a || b`), to wyrażenie `b` jest całkowicie pomijane. Czyli np. w wyrażeniu

```c++
if (root->left != nullptr && root->left->parent != root)
```

jeżeli `root->left != nullptr` ma wartość `false`, to cały warunek ma wartość `false`, a więc program nie sprawdza, czy ` root->left->parent != root`. Ma to duże znaczenie, bo przecież jeżeli  wyrażenie `root->left != nullptr` ma wartość `false`,  to `root->left == nullpr`, a w tym przypadku próba wyznaczenia wartości wyrażenia `root->left->parent` musi zakończyć się padem programu. Oznacza to też, że zasadniczo operatory `&&` i `||` nie są przemienne i zamiana kolejności ich argumentów zmienia sens programu. Na przykład zamiana powyższego warunku na 

```c++ 
if (root->left->parent != root && root->left != nullptr)
```

miałaby dla programu fatalne konsekwencje. 

Wszystkie instrukcje warunkowe (`if`) w programie z poprzedniej części ("Weryfikacja niezmienników drzewa") wykorzystują właściwość *short-circuit evaluation*. Co więcej, w profesjonalnych programach często pomija się jawne sprawdzanie, czy dany wskaźnik jest niezerowy, czyli zamiast `if(root->left != nullptr)`, sporo programistów pisze zwięźle `if (root->left)`, a zamiast `if (root->left == nullptr)` moglibyśmy ujrzeć krótsze  `if (!root->left)`.

Innymi słowy, instrukcja 

```c++
if (a() && b()) 
    c();
```

jest równoważna instrukcji

```c++
if (a())
    if(b())
        c();
```

Z kolei instrukcja

```c++
if (a() || b()) 
    c();
```

jest równoważna instrukcji

```c++
if (a())
    c();
else if (b())
    c();
```

#### Wyświetlanie kluczy w drzewie BST: *pre order*, *in order*, *post order* 

Załóżmy, że chcemy wyświetlić wszystkie klucze przechowywane w danym drzewie. [Przejście węzłów drzewa](https://pl.wikipedia.org/wiki/Przechodzenie_drzewa) można wykonać na wiele sposobów. Powszechnie stosuje się trzy, z których każdy jest inną realizacją przeszukiwania typu DFS (*deep-first search*, przeszukiwanie wgłąb) :

- ***pre order*** - w tej metodzie w każdym (pod)drzewie wyświetl kolejno:
  - klucz jego korzenia
  - rekurencyjnie całe lewe poddrzewo 
  - rekurencyjnie całe prawe poddrzewo.
- ***in order*** -  w tej metodzie w każdym (pod)drzewie wyświetl kolejno: 
  - rekurencyjnie całe lewe poddrzewo 
  - klucz jego korzenia
  - rekurencyjnie całe prawe poddrzewo.
- ***post order*** -  w tej metodzie w każdym (pod)drzewie wyświetl kolejno:
  - rekurencyjnie całe lewe poddrzewo 
  - rekurencyjnie całe prawe poddrzewo.
  - klucz jego korzenia

Jak widać, każda z powyższych definicji jest rekurencyjna, dlatego łatwo przenosi się je na kod źródłowy:

##### Metoda *pre order* 

```c++
void print_pre_order(const Node* root)
{
    if (root == nullptr)
        return;
    std::cout << root->key << " ";
    print_pre_order(root->left);
    print_pre_order(root->right);
}
```

##### Metoda *in order*

```c++
void print_in_order(const Node* root)
{
    if (root == nullptr)
        return;
    print_in_order(root->left);
    std::cout << root->key << " ";
    print_in_order(root->right);
}
```

##### Metoda post order

```c++ 
void print_post_order(const Node* root)
{
    if (root == nullptr)
        return;
    print_post_order(root->left);
    print_post_order(root->right);
    std::cout << root->key << " ";
}
```

Jak widać, są one bardzo podobne do siebie, różnią się tylko chwilą, w której wyświetla się klucz korzenia danego (pod)drzewa. 

#### Zadania

1. Zaimplementuj funkcję `size` zwracającą liczbę węzłów drzewa BST.
2. Zaimplementuj funkcję `sum` zwracającą sumę elementów drzewa BST.
3. Zaimplementuj funkcję `min` zwracającą wartość najmniejszego klucza w drzewie BST. Podaj rozwiązanie rekurencyjne i niewykorzystujące rekurencji. 
4. Zaimplementuj funkcję `max` zwracającą wartość największego klucza w drzewie BST. Podaj rozwiązanie rekurencyjne i niewykorzystujące rekurencji. 
5. Zaimplementuj funkcję `find`  zwracającą wskaźnik do klucza o danej wartości `n` lub `nullptr`, jeżeli takiego węzła nie ma.
6. Zaimplementuj funkcję `height` zwracającą [wysokość drzewa](https://pl.wikipedia.org/wiki/Drzewo_(matematyka)) BST. Wysokość drzewa to długość najdłuższej ścieżki od korzenia do liścia. Dla pustego drzewa BST niech ta funkcja zwraca `-1`. 
7. Zaimplementuj funkcję zero zerującą wszystkie klucze drzewa BST.
8. Zaimplementuj funkcję `iota`, wypełniającą drzewo BST kolejnymi liczbami naturalnymi, z zachowaniem jego struktury i właściwości BST. Uwaga: bardzo łatwo napisac funkcję o sygnaturze `void iota(Node*& root, int & counter)`. Jej drugi argument jest kolejną liczbą, którą należy zapisać w kolejnym węźle drzewa. Za jej pomocą można nastepnie zaimplementować wygodny w użyciu wrapper:
   ```c++
   void iota(Node*& root)
   {
    int n= 1;
    iota(root, n);
   }
   ```
9. Zaimplementuj funkcję `insert`, dodającą do drzewa BST węzeł o danej wartości `n`.
9. Zaimplementuj funkcję `remove`, usuwającą z drzewa BST węzeł wskazywany przez wskaźnik (to zadanie może być nieco żmudne, bo trzeba osobno rozpatrzyć kilka wariantów).        

 
