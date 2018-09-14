

#include <string>

#include <iostream>

int main()
{
  int* i = new int();
  double* d = new double();

//  std::string* s = new std::string();
  std::string s;

  std::cout << i << ", " << d << ", " << &s << ", "
            << static_cast<const void*>(s.data()) << std::endl;
}
