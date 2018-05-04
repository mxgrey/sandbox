
#include <string>
#include <iostream>


void foo(const std::string& str)
{
  std::cout << str << std::endl;
}

void foo(std::string& str)
{
  str += " derp";
  std::cout << str << std::endl;
}

void foo(std::string str)
{
  std::cout << str << std::endl;
}

//void foo(const std::string str)
//{
//  std::cout << str << std::endl;
//}

int main()
{
//  foo(std::string());
}
