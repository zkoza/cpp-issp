### Programowanie dynamiczne

Programowanie dynamiczne jest bardzo wdzięcznym tematem do nauki programowania w C++. Uwaga: nie zamierzam tu tłumaczyć dogłębnie, co to jest programowanie dynamiczne. Tu przedstawiam tylko wybrane przykłady problemów, które można zaprogramować w sposób właściwy dla języka C++, wykorzystując do tego te jego cechy, które stanowią o jego sile i wciąż niemalejącej popularności. 

Zasadniczo, w programowaniu dynamicznym mamy do czynienia z trzema elementami:

1. Problem można rozwiązać za pomocą rekurencji
2. Problem wymaga zastosowania techniki zwanej spamiętywaniem (ang. *memoisation*).
3. Problem dotyczy konstrukcji optymalnego algorytmu znajdującego rozwiązanie jakiegoś zadania optmalizacyjnego (np. znalezienia najkrótszej drogi dla listonosza mającego dostarczyć przesyłkę do *N* odbiorców).

Trzeci z powyższych punktów to tylko definicja typowego obszaru zastosowań, kluczowe są dwa pierwsze. Ich implementacja, jak już wspomniałem, stanowi wdzięczny pretekst do pokazania siły C++ w jego naturalnym środowisku. Dwa pierwsze punkty są też ciekawe, gdyż rekurencja kojarzy się z funkcją, zaś spamiętywanie (o którym poniżej) to technika, dzięki której funkcja otrzymuje coś w rodzaju własnej, trwałej pamięci. A jak wiemy, funkcja + pamięć to funkcja + środowisko, a to z kolei wygląda jak typowe domknięcie (ang. *closure*), jakim jest, m.in. każde wyrażenie lambda (w C++). I ten związek programowania dynamicznego z projektowaniem i implementacją domknięć interesować nas będzie najbardziej. 

W dalszej części kwestię implementacji algorytmów programowania dynamicznego przedstawię na dwóch przykładach:

1. Funkcja rekurencyjna wyznaczająca wartość symbolu Newtona, $N \choose k$.
2. Funkcja rekurencyjna rozwiązująca tzw. problem plecakowy (i to już będzie klasyczne zagadnienie programowania dynamicznego, którego rozwiązanie zawierać będzie wszystkie trzy przedstawione powyżej elementy). 

