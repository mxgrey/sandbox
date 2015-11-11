
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

  template <class T> struct type { };

  std::string mString;

};

template <class Alt1, class Alt2>
class Joiner : public virtual Alt1, public virtual Alt2
{
public:

  template <class T>
  void print()
  {
    print(Base::type<T>());
  }

protected:

  template <class T>
  void print(Base::type<T>)
  {
    if(Alt1::specialized(Base::type<T>()))
      return Alt1::print(Base::type<T>());

    Alt2::print(Base::type<T>());
  }

  template <class T>
  static constexpr bool specialized(Base::type<T>)
  {
    return (Alt1::specialized(Base::type<T>()) || Alt2::specialized(Base::type<T>()));
  }
};

template <class... OtherSpecs>
class Alternative { };

template <typename Spec>
class Alternative<Spec> : public virtual Base
{
public:

  template <class T>
  void print()
  {
    print(type<T>());
  }

protected:

  template <class T>
  void print(type<T>)
  {
    Base::template print<T>();
  }

  void print(type<Spec>)
  {
    std::cout << "Alternative -> " << typeid(Spec).name() << std::endl;
  }

  template <class T>
  static constexpr bool specialized(type<T>)
  {
    return false;
  }

  static constexpr bool specialized(type<Spec>)
  {
    return true;
  }

};

template <typename Spec, typename... OtherSpecs>
class Alternative<Spec, OtherSpecs...> :
    public virtual Joiner< Alternative<Spec>, Alternative<OtherSpecs...> >
{
public:

protected:

};

class Derived : public virtual Alternative<Base, int, double>
{

};

class Derived2 : public virtual Alternative<Base, float>
{

};

class Joined : public virtual Joiner<Derived, Derived2>
{

};

class Derived3 : public virtual Alternative<Base, int>
{

};

class Diamond : public virtual Joiner<Derived, Derived3>
{

};

int main()
{
  std::cout << "B:" << std::endl;
  Base B;
  B.print<int>();

  std::cout << "\nD:" << std::endl;
  Derived D;
  D.print<int>();
  D.print<double>();
  D.print<std::string>();
  D.print<float>();

  std::cout << "\nD2:" << std::endl;
  Derived2 D2;
  D2.print<float>();

  std::cout << "\nJ:" << std::endl;
  Joined J;
  J.print<int>();
  J.print<float>();

  std::cout << "\nDiamond:" << std::endl;
  Diamond diamond;
  diamond.print<float>();
  diamond.print<int>();
}
