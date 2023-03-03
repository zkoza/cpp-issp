

### Charakterystyczne cechy arytmetyki zmiennopozycyjnej

#### Zaokrąglanie

Jak już wiemy, z reguły liczby rzeczywiste są zapisywane w zmiennych zmiennopozycyjnych w postaci zaokrąglonej. Przypomnijmy zapis liczby 0.2:

![](/home/zkoza/Pulpit/Dydaktyka/aaa_ProgCPP/github/cpp-issp/img/02/0.2.png)

Liczba 0.2 jest zapisywana w zmiennych typu `float` jako swoje przybliżenie o wartości dziesiętnej $0.20000000298023223876953125$. Komputer zapisuje więc $0.2$ jako [liczbę wymierną](https://www.wolframalpha.com/input?i=0.20000000298023223876953125+as+rational+number++)  $13421773 \over 67108864$, czyli 
$$
\mbox{float}(0.2) = 2^{-3}\cdot\frac{13421773}{2^{23}}
$$
gdzie wyciągnąłem przed ułamek wyrażenie wykładnicze $2^{-3}$, które na powyższym rysunku figuruje w sekcji "Exponent". Mantysa na tym rysunku, po zinterpretowaniu jako liczby całkowitej zapisanej w układzie dwójkowym, ma wartość 5 033 165, nie zapominajmy jednak, że pierwszy bit części ułamkowej jest zawsze ukryty. Odpowiada on wartości $2^{23} = 8 388 608$.  Po dodaniu jej do 5033165 otrzymujemy, zgodnie z powyższym wzorem, 13421773. Oczywiście nieprzypadkowo wykładnik w mianowniku równy jest liczbie bitów mantysy (tu: 23).

#### Epsilon maszynowy

Z powyższego wynika, że każdej liczbie zmiennopozycyjnej odpowiada cały przedział liczb rzeczywistych, które ta liczba reprezentuje. Szerokość tego przedziału wzrasta dla dużych liczb i maleje dla małych. Jego względna szerokość jest jednak mniej więcej stała - oznaczamy ją grecką literą *ε*. Ma ona duże znaczenie, gdyż błąd zaokrąglenia nie przekracza *ε/2*:      
$$
   x - (\epsilon/2) x \le \textrm{floating-point-representation}(x) \le x + (\epsilon/2) x
$$

Wartość epsilona maszynowego równa jest $2^{-B}$, gdzie B to liczba cyfr dwójkowych mantysy. Łatwo się o tym przekonać, rozważając reprezentację bitową liczby `1.0f`. 
$$
   \mbox{float}(1) = 2^{0}\cdot\frac{8~388~608}{8~388~608} = 2^{0}\cdot\frac{2^{23}}{2^{23}}
$$
Kolejna liczba, jaką można reprezentować w typie `float`, to
$$
\mbox{float}(1 + \epsilon) = 2^{0}\cdot\frac{8~388~609}{8~388~608} = 2^{0}\cdot\frac{2^{23} + 1}{2^{23}}
$$
skąd ε = $1/2^{23} \approx 10^{-7}$. 

| typ                           | ε         |
| ----------------------------- | --------- |
| `float`             (32 bity) | ≈ 1.2e-07 |
| `double`           (64 bity)  | ≈ 2.2e-16 |
| `long double`  (80 bitów)     | ≈ 1.1e-19 |

Wynika stąd, że liczby typu `float` mają ok. 7 cyfr znaczących, typ `double` ma ok. 16 cyfr znaczących, a `long double` w naszych PC-tach ma ok. 19 cyfr znaczących.  Nie używaj `float` i `long double`, jeśli nie masz naprawdę dobrych powodów, by to robić.  Pierwszy jest za mało dokładny, większa dokładność drugiego raczej do niczego nie będzie ci potrzebna, a ponadto ta dokładność jest "nieprzenośna": w kompilatorach Microsoftu typ `long double` jest równoważny typowi `double`) 

#### Katastrofalne znoszenie się składników

Powiedzmy, że chcemy obliczyć z zasad pierwszych pochodną funkcji $f(x) = x$ w punkcie $x = 0.39$. Wiemy, że wartość dokładna tej pochodnej to 1. Korzystamy ze wzoru 
$$
   f'(x) = \lim_{h\to 0} \frac{f(x + h) - f(x)}{h}.
$$
Spróbujmy wyznaczyć wartość ilorazu po prawej stronie tej zależności dla jakiegoś "małego" *h*. Np. dla h = 0.000002. Problem w tym, że 

- wartość 0.39 zostanie zaokrąglona do `0.38999998569488525390625` ,
- wartość 0.000002 będzie zaokrąglona do `0.000001999999994950485415756702423095703125`,
- ich suma (czyli $f(x + h)$) zostanie zaokrąglona do `0.3900019824504852294921875`,  
- różnica  $f(x + h) - f(x)$ zostanie zaokrąglona do `0.0000019967555999755859`,
- Iloraz $(f(x + h) - f(x))/h$ zostanie zaokrąglony do `0.99837791919708251953125`.  

Każda z liczb użytych w obliczeniach była zapisana z  dokładnością do ok. 7 cyfr znaczących, jednak w wyniku otrzymaliśmy wartość błędną już na 3 cyfrze znaczącej (po przecinku), `0.998...`. Jeśli teraz, zgodnie z definicją matematyczną, spróbujemy jeszcze bardziej zmniejszyć *h*, to dostawać będziemy  liczby coraz bardziej odległe od prawdziwej granicy. Jeśli *h* zejdzie poniżej wartości *x*ε, to w liczniku otrzymamy po prostu 0, więc naszym "przybliżeniem" pochodnej będzie zero: żadna cyfra wyniku nie będzie poprawna. 

```c++
#include <iostream>

int main()
{
    const float x = 0.39f;
    float h = 0.2;
    std::cout.precision(7);
    for (int i = 0; i < 9; i++)
    {
        h /= 10.0f;
        float fx = x;
        float fxh = x + h;
        std::cout << "h = " << h << "\t => f'(x) = " << (fxh - fx) / h << "\n";
    }
}
```

    Program ten wyświetla następujące wyniki:

   ```txt\
h = 0.02	 => f'(x) = 1.000001
h = 0.002	 => f'(x) = 1.000002
h = 0.0002	 => f'(x) = 1.000017
h = 2e-05	 => f'(x) = 0.999868
h = 2e-06	 => f'(x) = 0.9983779
h = 2e-07	 => f'(x) = 1.043081
h = 2e-08	 => f'(x) = 1.490116
h = 2e-09	 => f'(x) = 0
h = 2e-10	 => f'(x) = 0
   ```

Jak widzimy, zmniejszanie wartości `h` tylko zwiększa błąd, z jakim wyznaczamy wartość pochodnej (to nie jest regułą!) i błąd ten nigdy nie jest mniejszy niż ok. 7 cyfr znaczących. Co by się stało, gdybyśmy tak dobrali wartości `h`, by zminimalizować (lub wręcz wyeliminować) zaokrąglenia? 

   ```c++
int main()
{
    const float x = 0.39f;
    float h = 0.25;
    std::cout.precision(7);
    for (int i = 0; i < 9; i++)
    {
        h /= 8.0f;
        float fx = x;
        float fxh = x + h;

        std::cout << "h = " << h << "\t => f'(x) = " << (fxh - fx) / h << "\n";
    }
}
   ```

Wynik:

   ```txt
h = 0.03125	 => f'(x) = 1
h = 0.00390625	 => f'(x) = 1
h = 0.0004882812	 => f'(x) = 1
h = 6.103516e-05	 => f'(x) = 1
h = 7.629395e-06	 => f'(x) = 1
h = 9.536743e-07	 => f'(x) = 1
h = 1.192093e-07	 => f'(x) = 1
h = 1.490116e-08	 => f'(x) = 0
h = 1.862645e-09	 => f'(x) = 0
   ```

Katastrofalne znoszenie się składników można zilustrować też kolejnym przykładem. Załóżmy, że mamy kalkulator, który pracuje w układzie dziesiętnym i wyświetla jedną cyfrę przed i 4 po przecinku oraz 2 cyfry wykładnika dziesiętnego. Powiedzmy, że chcemy od 1/3 odjąć 0.33332. 1/3 w takim kalkulatorze zostanie zaokrąglona do 3.3333E-01,  a druga liczba zostanie zapisana dokładnie jako 3.3332E-01. Kalkulator wyświetli ich różnicę w postaci  1.0000E-05, tymczasem wartość dokładna zaokrąglona do możliwości tego kalkulatora to 1.3333E-05. Zamiast pięciu cyfr znaczących mamy tylko jedną. Cztery pierwsze uległy "katastrofalnemu zniesieniu się", na ich miejsce kalkulator wprowadził po prostu zera. Jeżeli takie zniesienie składników wystąpi w programie wielokrotnie, to otrzymane wyniki mogą nie mieć nic wspólnego z rzeczywistością.  

#### Przemienność

Tu w zasadzie nie ma problemu: `x + y` to to samo, co `y + x`. To samo dotyczy mnożenia. Pamiętamy jednak, że `f(x) + g(x)` może nie być równoważne `g(x) + f(x)`, jeżeli funkcje te generują efekty uboczne. 

#### (Nie-)łączność dodawania

Dodawanie liczb zmiennopozycyjnych nie jest łączne: wartość sumy więcej niż 2 składników  zależy od kolejności, w jakiej wykonujemy tę operację. Prosty przykład: wartość sumy 1.0 + (-1.0) + 1.0e-20  zależy od tego, czy dodawanie wykonujemy w kolejności od lewej do prawej, czy od prawej do lewej. 

- od lewej do prawej: `(1.0 + (-1.0)) + 1.0e-20   ==   0 + 1e-20     ==   1e-20`
- od lewej do prawej:   `1.0 + ((-1.0) + 1.0e-20)  ==   1.0 + (-1.0)  ==   0.0`  

### Podsumowanie

Poprawne posługiwanie się liczbami zmiennopozycyjnymi jest trudniejsze, niż mogłoby się to wydawać. Wszystkie przedstawione tu cechy arytmetyki na tych liczbach, czy raczej reprezentujących liczby obiektach, dotyczą każdego języka programowania. Proszę zapamiętać hasła "epsilon maszynowy", "zaokrąglanie", "znoszenie się składników" i "zależność wyniku od kolejności dodawania i odejmowania". 

#### Dalsza lektura

- Wikipedia [Floating point arithmetic](https://en.wikipedia.org/wiki/Floating-point_arithmetic) 
- Symulator reprezentacji typu float,  [IEEE-754 Floating Point Converter](https://www.h-schmidt.net/FloatConverter/IEEE754.html) 
- Symulator reprezentacji typu double oraz operacji na nim [IEEE 754 Calculator](http://weitz.de/ieee/) (dobry do badania zjawiska znoszenia się składników)