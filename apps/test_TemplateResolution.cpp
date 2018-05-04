
#include <iostream>

struct SomeClass
{
  template <typename T>
  bool someFunction()
  {
    return true;
  }

  bool someFunction()
  {
    return true;
  }
};

struct SomeStruct
{
  double val;

  template <typename Return>
  Return operation()
  {
    return val;
  }
};


int main()
{
  SomeClass something;
  something.someFunction();

  something.someFunction<double>();

  SomeStruct thing;
  thing.val = 1;
  const double& ref_val = thing.operation<double&>();
  const double& val = thing.operation<double>();

  thing.val += 1;

  std::cout << "Outputs: " << ref_val << " | " << val << std::endl;

}
