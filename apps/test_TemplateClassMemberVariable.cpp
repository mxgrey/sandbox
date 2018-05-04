
#include <iostream>

template<typename Scalar>
struct SomeType
{
  Scalar val;

  SomeType() : val(3) { }
};

template<typename Scalar>
struct C
{
  SomeType<Scalar> myType;
};

template<typename S, SomeType<S> C<S>::*ptr>
struct TemplatedThing
{
  TemplatedThing(const C<double>& object)
  {
    std::cout << (object.*ptr).val << std::endl;
  }
};


int main()
{
  C<double> someClass;
  someClass.myType.val = 50;
  TemplatedThing<double, &C<double>::myType> instance(someClass);
}
