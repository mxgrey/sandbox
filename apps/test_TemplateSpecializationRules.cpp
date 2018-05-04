
#include <iostream>

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


int main()
{
  SomeClass<int> something;
}
