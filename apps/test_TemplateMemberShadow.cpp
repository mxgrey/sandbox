
#include <iostream>
#include <typeinfo>
#include <type_traits>

class Base
{
public:

  Base()
  {
    mString = "Base";
  }

  template <class T>
  void print()
  {
    std::cout << mString << " -> " << typeid(T).name() << std::endl;
  }

protected:

  std::string mString;

};

template <class BaseT, class... OtherSpecs>
class Alternative : public virtual BaseT
{

};

template <class BaseT, typename Spec, typename... OtherSpecs>
class Alternative<BaseT, Spec, OtherSpecs...> : public Alternative<BaseT, OtherSpecs...>
{
  template <class T> struct type { };

public:

  using Parent = Alternative<BaseT, OtherSpecs...>;

  template <class T>
  void print()
  {
    print(type<T>());
  }

protected:

  template <class T>
  void print(type<T>)
  {
    Parent::template print<T>();
  }

  void print(type<Spec>)
  {
    std::cout << "Alternative -> " << typeid(Spec).name() << std::endl;
  }

};

class Derived : public Alternative<Base, int, double, std::string>
{

};


int main()
{
  Base B;
  B.print<int>();

  Derived D;
  D.print<int>();
  D.print<double>();
  D.print<std::string>();
  D.print<float>();

}
