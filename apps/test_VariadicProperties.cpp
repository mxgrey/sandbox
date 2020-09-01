
#include <memory>

#include <iostream>
#include <typeindex>

class FooOne
{
  struct Properties
  {
    using Container = FooOne;
  };
};

class FooTwo
{
  struct Properties
  {
    using Container = FooTwo;
  };
};

class FooThree
{
  struct Properties
  {
    using Container = FooThree;
  };
};

class Bar
{

};

//using BarPtr = std::shared_ptr<Bar>;

//class Manager
//{
//  template <typename... Args>
//  std::shared_ptr<Manager> create(const BarPtr& bar);

//  template <typename... Args>
//  std::shared_ptr<Manager> create(
//      const BarPtr& bar, )

//protected:

//  BarPtr mBar;

//};

int main()
{
  std::cout << typeid(std::shared_ptr<void>).name() << std::endl;
}
