## Statyczne i dynamiczne łączenie funkcji

Droga od kodu źródłowego w praktycznie dowolnym kompilowanym języku języku programowania jest skomplikowana. Chcielibyśmy, by programy były poprawne, by efektywnie wykorzystywały dostępne  zasoby komputera i by ich wykonywanie było bezpieczne - uwzględniając zarówno błędy nieświadome, jaki i próby zhakowania systemu. Na szczęście tak jak kierowca nie musi znać budowy silnika, by móc jeździć samochodem, tak programista nie musi znać wszystkich szczegółów, jakie dzielą kod źródłowy od gotowego programu załadowanego do pamięci operacyjnego i uruchomionego. Niemniej, kilka takich szczegółów technicznych warto znać. Jednym z nich jest łączenie (ang. *binding*).

Jak zapewne wiemy, język C++ jest przystosowany do tworzenia bibliotek, co przejawia się tym, że można w nim kompilować różne pliki źródłowe zupełnie niezależnie od siebie.   Rozpatrzmy trywialny przykład programiku składającego się z dwóch plików źródłowych (`inc.cpp` i `main.cpp`) oraz jednego pliku nagłówkowego (`inc.h`):

```c++
// plik inc.h
#ifndef INC_H
#define INC_H

void inc(int & n); 

#endif
```

 ```C++
 // plik inc.cpp
 #include "inc.h"
 
 void inc(int & n)
 {
    n++;
 }
 ```

```C++
// plik main.cpp
#include "inc.h"

void dec(int & n)
{
   n--;
}

int main()
{
  int k = 0;
  inc(k);
  dec(k);
  return k;
}
```

 W przykładzie tym funkcja `main` korzysta z dwóch prostych funkcji, `inc` i `dec`, z których pierwsza zapisana jest w osobnym pliku, a druga - w tym samym, co `main`. Każdy z plików źródłowych możemy skompilować niezależnie od siebie:

```bash
> g++ main.cpp -g -c
> g++ inc.cpp -g -c
```

W powyższych poleceniach opcja `-g` umieszcza w plikach wynikowych informacje o kodzie źródłowym, a opcja  `-c` powoduje zatrzymanie kompilatora po pierwszej fazie kompilacji. Opcję tę stosuje się zawsze, gdy stosuje się taką rozłączną kompilację osobnych plików źródłowych. W efekcie powyższych poleceń otrzymujemy dwa pliki (tzw. pliki obiektowe) o rozszerzeniu `*.o`: 

  ```bash
  > ls 
  inc.cpp  inc.h  inc.o  main.cpp  main.o
  ```

Poleceniem `objdump` z opcjami `-dSC` możemy wyświetlić kod maszynowy oraz kod w asemblerze. Najpierw plik `inc.o`:

```assembly
> objdump inc.o -dSC
inc.o:     file format elf64-x86-64
Disassembly of section .text:
0000000000000000 <inc(int&)>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
   8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
   c:	8b 00                	mov    (%rax),%eax
   e:	8d 50 01             	lea    0x1(%rax),%edx
  11:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  15:	89 10                	mov    %edx,(%rax)
  17:	90                   	nop
  18:	5d                   	pop    %rbp
  19:	c3                   	ret
```

a teraz plik `main.o`:

```assembly
> objdump main.o -dSC
main.o:     file format elf64-x86-64
Disassembly of section .text:
0000000000000000 <dec(int&)>:
#include "inc.h"

void dec(int & n)
{
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
   n--;
   8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
   c:	8b 00                	mov    (%rax),%eax
   e:	8d 50 ff             	lea    -0x1(%rax),%edx
  11:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  15:	89 10                	mov    %edx,(%rax)
}
  17:	90                   	nop
  18:	5d                   	pop    %rbp
  19:	c3                   	ret

000000000000001a <main>:

int main()
{
  1a:	55                   	push   %rbp
  1b:	48 89 e5             	mov    %rsp,%rbp
  1e:	48 83 ec 10          	sub    $0x10,%rsp
  22:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  29:	00 00 
  2b:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  2f:	31 c0                	xor    %eax,%eax
  int k = 0;
  31:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
  inc(k);
  38:	48 8d 45 f4          	lea    -0xc(%rbp),%rax
  3c:	48 89 c7             	mov    %rax,%rdi
  3f:	e8 00 00 00 00       	call   44 <main+0x2a>
  dec(k);
  44:	48 8d 45 f4          	lea    -0xc(%rbp),%rax
  48:	48 89 c7             	mov    %rax,%rdi
  4b:	e8 00 00 00 00       	call   50 <main+0x36>
  return k;
  50:	8b 45 f4             	mov    -0xc(%rbp),%eax
}
  53:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  57:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
  5e:	00 00 
  60:	74 05                	je     67 <main+0x4d>
  62:	e8 00 00 00 00       	call   67 <main+0x4d>
  67:	c9                   	leave
  68:	c3                   	ret
```

Nie musimy znać assemblera, by zrozumieć kilka najważniejszych kwestii związanych z omawianym tu łączeniem.  Po pierwsze, pierwsza kolumna zawiera liczby zakończone dwukropkiem. Są to adresy kodu wykonywalnego. Co ciekawe, adresy te w obu przypadkach zaczynają się od wartości `0`, tak jak by funkcje `inc` i `dec` miały trafić  do tego samego obszaru pamięci operacyjnej. Tak jednak oczywiście nie jest. Te adresy są adresami względnymi, każdy z plików **.o* zawiera kod o adresach względnych względem pewnych   adresów, które zostaną ustalone *później*. Po drugie, wywołania funkcji  tłumaczone są na identyczny kod maszynowy

```assembly
e8 00 00 00 00
```

któremu odpowiada instrukcja assemblera `call`. Problem w tym, że te 4 bajty zerowe, `00 00 00 00` występujące po rozkazie `e8` powinny zawierać adres funkcji, która ma być wywołana. Tego adresu jednak na tym etapie nie ma, gdyż nie wiadomo jeszcze, do jakich konkretnych fragmentów pamięci operacyjnej trafią poszczególne fragmenty naszego programu. 

Połączmy teraz oba pliko obiektowe:

```bash
> g++ main.o inc.o
```

 W katalogu bieżącym pojawi się plik wykonywalny o standardowej nazwie `a.out`:

```bash
> ls 
a.out  inc.cpp  inc.h  inc.o  main.cpp  main.o
```

Zobaczmy, jak wygląda kod tego programu w rozpisce na assembler i kod maszynowy (fragment):

```assembly
> objdump  a.out -dSC
0000000000001139 <dec(int&)>:
#include "inc.h"

void dec(int & n)
{
    1139:	55                   	push   %rbp
    113a:	48 89 e5             	mov    %rsp,%rbp
    113d:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
   n--;
    1141:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1145:	8b 00                	mov    (%rax),%eax
    1147:	8d 50 ff             	lea    -0x1(%rax),%edx
    114a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    114e:	89 10                	mov    %edx,(%rax)
}
    1150:	90                   	nop
    1151:	5d                   	pop    %rbp
    1152:	c3                   	ret

0000000000001153 <main>:

int main()
{
    1153:	55                   	push   %rbp
    1154:	48 89 e5             	mov    %rsp,%rbp
    1157:	48 83 ec 10          	sub    $0x10,%rsp
    115b:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1162:	00 00 
    1164:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1168:	31 c0                	xor    %eax,%eax
  int k = 0;
    116a:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
  inc(k);
    1171:	48 8d 45 f4          	lea    -0xc(%rbp),%rax
    1175:	48 89 c7             	mov    %rax,%rdi
    1178:	e8 25 00 00 00       	call   11a2 <inc(int&)>
  dec(k);
    117d:	48 8d 45 f4          	lea    -0xc(%rbp),%rax
    1181:	48 89 c7             	mov    %rax,%rdi
    1184:	e8 b0 ff ff ff       	call   1139 <dec(int&)>
  return k;
    1189:	8b 45 f4             	mov    -0xc(%rbp),%eax
}
    118c:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    1190:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
    1197:	00 00 
    1199:	74 05                	je     11a0 <main+0x4d>
    119b:	e8 90 fe ff ff       	call   1030 <__stack_chk_fail@plt>
    11a0:	c9                   	leave
    11a1:	c3                   	ret

00000000000011a2 <inc(int&)>:
    11a2:	55                   	push   %rbp
    11a3:	48 89 e5             	mov    %rsp,%rbp
    11a6:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    11aa:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    11ae:	8b 00                	mov    (%rax),%eax
    11b0:	8d 50 01             	lea    0x1(%rax),%edx
    11b3:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    11b7:	89 10                	mov    %edx,(%rax)
    11b9:	90                   	nop
    11ba:	5d                   	pop    %rbp
    11bb:	c3                   	ret
```

Pierwsza kolumna, adres, mają już jednoznaczne wartości, noc się nie pokrywa. Co więcej, wszystkie funkcje asemblerowe `call` otrzymały poprawne wartości parametrów.  Np. instrukcja `dec(k);  została przetłumaczona jako 

```assembly
    117d:	48 8d 45 f4          	lea    -0xc(%rbp),%rax
    1181:	48 89 c7             	mov    %rax,%rdi
    1184:	e8 b0 ff ff ff       	call   1139 <dec(int&)>
```

Widzimy tu odwołanie do "poprawnego" adresu funkcji `dec(int&)`, czyli `1139`. Na tym etapie wszystkie funkcje `call` otrzymały poprawne wartości funkcji, które mają w tym miejscu być wywołane niezależnie od tego, czy ich definicje były w tym samym, czy różnych plikach. 

Pierwszą z powyżej opisanych faz translacji kodu źródłowego w C++ na kod maszynowy nazywa się **kompilacją**, (ang. *compilation*) a drugą - łączeniem (ang. *linking*), stąd też komunikaty diagnostyczne kompilatorów dzielą się na dwie podstawowe grupy: "*compiler errors/warnings*" i "*linker errors/warnings*". Ustalanie adresów wywoływanych funkcji w fazie łączenia plików obiektowych w plik wykonywalny to tzw.  łączenie (ang. *binding*). Łączenie obejmuje też m.in. ustalanie adresów obiektów globalnych, np. `std::cout` tak, by każdy fragment kodu odwoływał się do tych samych zmiennych. W powyższym przykładzie linker zakończył pracę: adresy wszystkich funkcji (względem pewnego adresu ustalającego początek segmentu zawierającego kod wykonywalny programu) są zapisane w pliku wykonywalnym. O takiej sytuacji mówi się, że ***łączenie*** zostało wykonane ***statycznie***, czyli w fazie kompilacji, a więc przed uruchomieniem programu. 

Łączenie statyczne (czyli wykonane podczas tworzenia pliku wykonywalnego) ma dużo zalet (np. prostotę), ale nie pozbawione jest wad. Jedną z najpoważniejszych jest duplikowanie kodu w systemach wielozadaniowych. Chodzi o to, że zwykle, posługując się komputerami (smartfonami itp.), mamy uruchomionych kilka, kilkanaście, kilkadziesiąt programów realizujących identyczne operacje, np. wyświetlanie domyślnej belki tytułowej programów. Czyż nie lepiej byłoby ładować do pamięci komputera jedną kopię takich często używanych funkcji, a potem udostępniać ją różnym programom "na żądanie"? Taka opcja istnieje i nazywa się ***łączeniem dynamicznym***, czyli takim, które wykonywane jest *po* załadowaniu programu z dysku do pamięci komputera.

Wróćmy do naszego programu i skompilujmy go tak, by funkcja `inc(int&)` łączona była dynamicznie. W tym celu tworzymy plik z biblioteką dynamiczną (`inc.so`):

```bash
> g++ inc.cpp -fpic -c
> g++ -shared inc.o -o inc.so
```

Pierwsza z powyższych instrukcji tworzy plik `inc.o`, druga przekształca go w bibliotekę dynamiczną `inc.so`

```bash
inc.cpp  inc.h  inc.o  inc.so  main.cpp  main.o
```

Rozszerzenie `so` pochodzi od angielskiego określenia *shared object*. W systemie Windows pliki te znane są jako "de-el-elki", czyli pliki `*.dll`. Mając taki plik, możemy utworzyć plki wykonywalny naszego programu:

```bash
> g++ main.o inc.so
```

Zwróćmy uwagę na to, że w tym podejściu łączymy obiekt statyczny (`*.o`) z biblioteką dynamiczną (`*.so`). Utworzony w ten sposób plik wykonywalny do uruchomienia potrzebuje dostępu do pliku `inc.so`. Stąd też zwykła próba uruchomienia tego programu kończy się fatalnie:

```bash
> ./a.out
./a.out: error while loading shared libraries: inc.so: cannot open shared object file: No such file or directory
```

 Dzieje się tak dlatego, że program uruchomieniowy posiada ściśle określony zestaw katalogów, w których poszukuje bibliotek dynamicznych, i naszego katalogu roboczego tam nie ma. Można to "naprawić" nastepująco:

```bash
> LD_LIBRARY_PATH=. ./a.out
echo $?
0
```

Wykorzystałem tu zmienną środowiskową `LD_LIBRARY_PATH`, w której można zapisać dodatkowe, niestandardowe katalogi z bibliotekami dynamicznymi, a poprawność wykonania programu sprawdziłem komendą `echo $?` zwracającą kod zakończenia poprzedniej komendy.  