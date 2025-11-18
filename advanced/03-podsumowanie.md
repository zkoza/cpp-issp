### Biblioteka *ranges* - podsumowanie

Biblioteka *ranges* wprowadza do standardu C++ nowe pojęcia oraz nieco zmienia (rozszerza) stare. Do najważniejszych zmian zaliczyć można

- Oparcie *ranges* na konceptach - nowym mechanizmie diagnostyki błędów, który wprowadzono do języka w wersji C++20. 
- Rezygnacja z oparcia algorytmów generycznych wyłącznie na parach iteratorów (umownie: `[begin`, `end)`) i przejście na bardziej ogólne zakresy `[iterator, wartownik)` (czyli: `[iterator, sentinel)`). W bibliotece *ranges* zakresem może być 
  - tradycyjna para iteratorów `[begin, end)`  wyprowadzona np. z kontenerów (jak w STL)
  - para o umownej postaci `[begin, begin + size)`, w której zakres ma z góry określoną liczbę elementów, `size` - takie zakresy realizowane są np. przez widok `std::views::counted` 
  - para o postaci `[begin, predykat)`, gdzie `predykat` jest funkcją zwracającą wartość typu `bool`, która kończy zakres warunkowo (np. `std::views::take_while`)
  - para o postaci `[begin,...)` odpowiadająca ciągowi nieskończonemu (por.: `std::views::iota`). 
- Wprowadzenie do biblioteki projekcji, czyli operacji modyfikujących wartości, jakie algorytmy tej biblioteki "widzą" jako wartości obiektów przeglądanego zakresu. Pozwala to pisać krótszy, łatwiej zrozumiały, łatwiejszy w utrzymaniu kod naszych programów.

