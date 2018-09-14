
#include <iostream>

struct Base1
{
  virtual void foo(int i) const = 0;
};

struct Base2
{
  virtual void foo(double d) const = 0;
};

struct Implementation1 : virtual Base1
{
  void foo(int i) const override
  {
    std::cout << "integer: " << i << std::endl;
  }
};

struct Implementation2 : virtual Base2
{
  void foo(double d) const override
  {
    std::cout << "double: " << d << std::endl;
  }
};

struct User : virtual Implementation1, virtual Implementation2
{
  using Implementation1::foo;
  using Implementation2::foo;
};

struct Derived : User { };

int main()
{
  Derived der;
  der.foo(1);
  der.foo(2.3);
}
