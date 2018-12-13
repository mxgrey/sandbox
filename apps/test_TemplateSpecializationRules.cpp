
#include <iostream>
#include <vector>

template<typename T>
struct SomeClass
{
  SomeClass()
  {
    std::cout << "Generic template" << std::endl;
  }
};

template<>
struct SomeClass<bool>
{
  SomeClass()
  {
    std::cout << "Bool template" << std::endl;
  }
};

template<>
struct SomeClass<float>
{
  SomeClass()
  {
    std::cout << "Float template" << std::endl;
  }
};

template<>
struct SomeClass<const int>
{
  SomeClass()
  {
    std::cout << "const int template" << std::endl;
  }
};

template<>
struct SomeClass<unsigned int>
{
  SomeClass()
  {
    std::cout << "unsigned int template" << std::endl;
  }
};

template<typename Element, typename Allocator>
struct SomeClass<std::vector<Element, Allocator>>
{
  SomeClass()
  {
    std::cout << "std::vector<Element> template" << std::endl;
  }
};

template<typename T>
struct CustomAllocator : std::allocator<T> { };

int main()
{
  SomeClass<int> something;

  SomeClass<std::vector<int>> vector;
  SomeClass<std::vector<int, CustomAllocator<int>>> custom_allocator;
}
