# Wstęp

Niniejszy serwis zawiera notatki do zajęć *Programowanie w C++* dla studentów kierunku [ISSP](https://www.facebook.com/InformatykaStosowanaWFA/) na Uniwersytecie Wrocławskim.

***Notatki z pewnością zawsze będą niepełne***. Nawet gdybym kiedyś mógł i chciał napisać kolejną książkę o C++, to zanim bym te notatki uzupełnił w zadowalającym stopniu,  język C++ zmieni się na tyle, że robotę trzeba by zaczynać prawie od początku.

### Jak się uczyć C++?

Języka C++ uczymy się mniej więcej tak, jak języka angielskiego lub gry na fortepianie. Uczymy się  trochę i staramy się tę "trochę" stosować w swoich programach / grze na fortepianie. A potem więcej i więcej, tyle, ile będzie nam potrzebne. Nie nauczymy się programować z książek, instrukcji użytkownika, bryków czy blogów. Żeby dobrze grać na fortepianie, trzeba naprawdę dużo na nim grać.

Językiem angielskim można się posługiwać, znając tylko kilka tysięcy słów i kilka podstawowych "prawd gramatycznych". Z programowaniem jest tak samo. Jeśli po ok. roku nauki czegoś jeszcze o C++ się nie wie, to znaczy, że pewnie dotąd nie było to potrzebne, czyli strata żadna. Ja staram się tu poruszać tematy naprawdę ważne i mające zastosowanie także poza C++.

### Zalecana literatura

Liczba materiałów do nauki C++ jest gigantyczna. Trudno mi doradzić, które z nich są odpowiednie dla osób (w miarę) początkujących. Na pewno należy unikać materiałów, które przedstawiają C++ w wersji wcześniejszej niż C++11. W czasach, gdy podstawową metodą zdobywania wiedzy była lektura książek, studenci chwalili podręcznik Jerzego Grębosza

-  Jerzy Grębosz, [Opus Magnum C++](https://ifj.edu.pl/private/grebosz/opus.html).

To na pewno dobry podręcznik, ma jednak jedną wadę: 3 tomy, łącznie 1605 stron lektury, a do tego [uzupełnienie do standardu C++17](https://ifj.edu.pl/private/grebosz/misja_spis_tresci.pdf) liczące sobie, bagatela, kolejne 265 stron.

Dawno, dawno temu napisałem niespełna 300-stronicowy podręcznik, którego uważna lektura wówczas dawała duże szanse na pomyślne przejście rozmowy kwalifikacyjnej podczas aplikowania o pracę:

-  Zbigniew Koza, [Język C++. Pierwsze starcie](https://helion.pl/ksiazki/jezyk-c-pierwsze-starcie-zbigniew-koza,jcppps.htm).

Niestety, dziś zawartość tej książki to dużo za mało, w dodatku przedstawia ona "stary" C++ w wersji z roku 1998. Może jednak, dzięki zwięzłości i skoncentrowaniu na najważniejszych cechach języka, komuś się jeszcze przydać. Tak podstawowe aspekty języka, jak czas życia obiektu, wskaźnik, referencja, klasa, funkcja, funkcja wirtualna, iterator, kontener, algorytm czy obiekt funkcyjny - nie zmieniły się ani o jotę od momentu wprowadzenia ich do języka.

Problem w tym, że język C++ rozwija się wraz z rozwojem technik kompilatorowych i konkurencji ze strony innych języków programowania. Z każdą kolejną wersją (najbardziej znane: C++89, C++98, C++11, C++17 i C++20) usuwa się z niego niewiele, za to dodaje dużo lub wręcz mnóstwo (takie przełomowe wersje to C++98, C++11 i C++20). Nie da się go nauczyć na poziomie profesjonalnym w semestr ani w rok. Nie ma sensu i chyba nawet nie można "nauczyć się" całego C++. Niemal wszyscy używają go na zasadzie "korzystam z tego, co już potrafię i cały czas staram się uczyć nowych rzeczy, o ile widzę ich użyteczność w programach, które piszę".  Zgodnie z tą regułą, sam mam niezłe rozeznanie w większości (ale nie całości) standardu C++17 (z, jak się domyślamy, 2017 r.) i tylko wyrywkową znajomość standardu C++20 (bo nie dostrzegłem jeszcze we własnej pracy zastosowań większości z wprowadzonych tam zmian, które są naprawdę duże).

Materiałem referencyjnym, czyli dosłownie przedstawiającym standard(y) języka wraz z przykładami użycia jest serwis CppReference:

- [cppreference.com](https://en.cppreference.com/w/).

Jedyną jego wadą jest to, że standard opisywany jest językiem mocno technicznym, prawdopodobnie dość trudnym dla osób początkujących.

Każdy programista korzysta z serwisu StackOverflow

- [StackOverflow](https://stackoverflow.com/).

Wątpię, czy istnieje pytanie związane z programowaniem na podstawowym lub średnim poziomie, które nie posiada tam już co najmniej jednej trafiającej w sedno odpowiedzi.

Kanały na YouTube:

- [Cherno](https://www.youtube.com/playlist?list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb) - w tej chwili 101 krótkich filmów, widziałem kilka, autor wie, o czym mówi
- Niestety, nie polecam kursu Mirosława Zelenta, co nie znaczy, że na początku nie ma sensu z niego korzystać. Co nie ma sensu to udawanie, że na ten kurs nie natrafisz. Trafisz, bo jest tego bardzo dużo i jest to po polsku. Jeśli uważasz, że to kurs dla ciebie, to OK.

Inne materiały:

- [Kurs C++0x](https://cpp0x.pl/kursy/Kurs-C++/1). Zaleta: po polsku, autor zwykle wie, o czym pisze, ale treść tego serwisu w ciągu 10 lat była chyba rzadko odświeżana, tymczasem od momentu jego powstania C++ zdążył się nieco rozrosnąć.  

(*work in progress* :-)  )

### Warunki zaliczenia przedmiotu

- Podstawą uzyskania zaliczenia jest obecność na zajęciach oraz rozwiązanie dostatecznej liczby zadań

- Szczegółowe zasady podaje osoba prowadząca ćwiczenia

### Egzamin

Kiedyś był pisemny, ale odkąd zauważyłem, że wielu studentów ma kłopoty z jasnym wysławianiem się pisemnym, jest ustny. Zwykle trwa 2 dni.
