

#include <iostream>

int main()
{
  double someValue = 1.23;

  // fails to compile -- good!
//  auto noCapture = []()
//  {
//    std::cout << someValue << std::endl;
//  };

  auto refCapture = [&]()
  {
    std::cout << someValue << std::endl;
  };

  auto valueCapture = [=]()
  {
    std::cout << someValue << std::endl;
  };

  someValue = 3.21;

  refCapture();
  valueCapture();
}
