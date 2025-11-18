#### Testy na zrozumienie tematu

1) Funkcja `std::ranges::sort` ma dwa warianty:

```c++
template< std::random_access_iterator I, std::sentinel_for<I> S,
          class Comp = ranges::less, class Proj = std::identity >
requires std::sortable<I, Comp, Proj>
constexpr I
    sort( I first, S last, Comp comp = {}, Proj proj = {} );
```

drugi:

```c++
template< ranges::random_access_range R, class Comp = ranges::less,
          class Proj = std::identity >
requires std::sortable<ranges::iterator_t<R>, Comp, Proj>
constexpr ranges::borrowed_iterator_t<R>
    sort( R&& r, Comp comp = {}, Proj proj = {} );
```

- czy rozumiesz każdy element tych deklaracji, poza, być może deklaratorem `requires` oraz `borrowed_iterator`? W szczególności, jakie znaczenie mają tu następujace frazy
  - `template`
  - `template<argumenty>`
  - `ranges::random_access_iterator R`
  - `class Comp = ranges::less`
  - `constexpr`
  - `sort`
  - `R && r`
  - `Comp comp = {}`
  - `Proj proj = {}`
- czy rozumiesz, czym obie te funkcje `sort` się różnią?

2. Co robi ten program?

   ```c++
   #include <algorithm>
   #include <print>
   #include <ranges>
   #include <string>
   #include <vector>
   
   int main()
   {
       std::vector<std::string> v = {"ala", "ma", "kota"};
       auto w = v;
       std::ranges::sort(w);
       auto lambda = [](auto s) { return s.length(); };
       int len = 1 + std::ranges::max_element(v, {}, lambda)->length();
       for (auto [x, y] : std::views::zip(v, w))
           std::println("{1:{0}} {2:{0}}", len, x, y);
   }
   ```

   3. Co robi ten niezbyt czytelny fragment kodu?

      ```c++
      using std::ranges::for_each;
      using std::views::iota;
      
      for_each(iota(1, 50), [](int z)
      {
        for_each(iota(1, z), [=](int x)
        {
          for_each(iota(x, z), [=](int y)
          {
            if (x * x + y * y == z * z)
              std::println("{:2}^2 + {:2}^2 = {:2}^2", x, y, z);
          });
        });
      });
      ```

   4. Co robi ten fragment kodu?

      ```c++
      std::string text = "Siedem dziewcząt z Albatrosa, tyś jedyna";
      auto n = std::ranges::distance(text | std::views::split(' '));
      std::println("{}", n);
      ```

   5. Co robi ten fragment kodu?

      ```c++
      std::string text = "Siedem dziewcząt z Albatrosa, tyś jedyna";
      std::vector<std::string> v;
      for (const auto& s : text | std::views::split(' '))
      {
          v.emplace_back(std::string_view(s));
      }
      ```

      