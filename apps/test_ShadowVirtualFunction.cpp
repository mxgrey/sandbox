
#include <iostream>

struct Base
{
  virtual void foo()
  {
    std::cout << "Calling Base::foo()" << std::endl;
  }

  virtual void foo(double withAnArgument)
  {
    std::cout << "Calling Base::foo(" << withAnArgument << ")" << std::endl;
  }

};

struct Entity : public Base
{
  virtual void foo() override
  {
    std::cout << "Calling Entity::foo()" << std::endl;
  }
};


int main()
{
  Entity entity;
  entity.foo();

//  entity.foo(10.0); // compilation error
  entity.Base::foo(10.0); // perfectly fine
}
