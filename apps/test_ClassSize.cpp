
#include <iostream>
#include <string>

class SomeClass
{
public:

  const double d;
  const int i;
  const std::string s;

};

class SomeClassWithStatic
{
public:

  static const double d;
  static const int i;
  static const std::string s;

};

int main()
{
  std::cout << sizeof(SomeClass) << std::endl;
  std::cout << sizeof(SomeClassWithStatic) << std::endl;
}
