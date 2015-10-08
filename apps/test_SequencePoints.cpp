
#include <iostream>

bool compare(int i1, int i2)
{
  return i1 < i2;
}

int sequence_point(int i)
{
  return i;
}

struct Printer
{
  template <typename T>
  Printer& print(const T& item)
  {
    std::cout << item << std::endl;
    return *this;
  }
};

int main(int , char* [])
{
  Printer printer;
  int i = 0;
  printer.print(++i).print(++i).print(++i);
//  std::cout << ++i << ++i << ++i << std::endl;
}
