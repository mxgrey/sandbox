
#include <iostream>
#include <typeinfo>
#include <tuple>

class F1 { };
class F2 { };
class F3 { };
class F4 { };
class F5 { };
class F6 { };
class F7 { };

template <typename... FeaturesT>
class List
{
public:

  using Features = std::tuple<FeaturesT...>;

};


template <typename... FeaturesT>
struct List<std::tuple<FeaturesT...>>
{
  using Features = std::tuple<FeaturesT...>;
};

template <typename T>
struct GetFeatures
{
  using Features = typename T::Features;
};

template <typename... Lists>
class CombineLists
{
public:
  using Result = List<decltype(std::tuple_cat(typename GetFeatures<Lists>::Features()...))>;
};


using List1 = List<F1, F2>;
using List2 = List<F3, F4>;
using List3 = List<F5, F6, F7>;

using FullList = CombineLists<List1, List2, List3>::Result;

int main()
{
  std::cout << typeid(FullList).name() << std::endl;
}
