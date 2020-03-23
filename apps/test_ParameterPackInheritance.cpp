
#include <iostream>

struct Foo
{
  struct Printer
  {
    void foo()
    {
      std::cout << "foo" << std::endl;
    }
  };
};

struct Bar
{
  struct Printer
  {
    void bar()
    {
      std::cout << "bar" << std::endl;
    }
  };
};

struct Ort
{
  struct Printer
  {
    void ort()
    {
      std::cout << "ort" << std::endl;
    }
  };
};

template <typename T>
struct Wrapper : public T::Printer
{

};

template<typename... Features>
struct Compose : public Wrapper<Features>...
{
  // Do nothing
};


int main()
{
  auto test = Compose<Foo, Bar, Ort>();

  test.foo();
  test.bar();
  test.ort();
}
