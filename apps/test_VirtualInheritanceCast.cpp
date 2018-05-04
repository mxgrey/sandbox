
#include <iostream>
#include <array>

class LowBase
{
public:
  virtual void testFunction1() = 0;
  virtual void testFunction2() = 0;
};

class SecondBase
{
public:
  virtual void otherFunction1(double val) = 0;
};

class MidBase1 : virtual public LowBase
{
public:
  void testFunction1() override
  {
    std::cout << "Called test function 1" << std::endl;
  }
};

class MidBase2 : virtual public LowBase
{
public:
  void testFunction2() override
  {
    std::cout << "Called test function 2" << std::endl;
  }
};

class Derived : public MidBase1/*, public MidBase2*/
{
public:
  void testFunction1() override final
  {
    std::cout << "Called most derived version of test function 1" << std::endl;
  }
};

class DoubleDerived : public SecondBase, public Derived
{
public:
  void testFunction2() override
  {
    std::cout << "Called most derived version of test function 2" << std::endl;
  }

  void otherFunction1(double val) override
  {
    std::array<double, 1000> array;
    for(std::size_t i=0; i < array.size(); ++i)
      array[i] = static_cast<double>(0xDEADBEEF);

    std::cout << "Calling most derived version of other function 1 with value "
              << val << " | Deadbeef: " << array[0] << std::endl;
  }
};

class Diamond : public virtual MidBase1, public virtual MidBase2
{
public:

};

class DiamondCrown : public virtual Diamond
{
public:

};

int main()
{
  void* voidInstance = new DoubleDerived;
  std::cout << "void: " << voidInstance << std::endl;

  //  DoubleDerived* derived = static_cast<LowBase*>(base);
    DoubleDerived* derived = static_cast<DoubleDerived*>(voidInstance);

//  LowBase* base = static_cast<LowBase*>(voidInstance);
//  std::cout << "FirstBase: " << base << std::endl;

  LowBase* base = static_cast<LowBase*>(derived);
  std::cout << "LowBase: " << base << std::endl;

  SecondBase* second = static_cast<SecondBase*>(voidInstance);
  std::cout << "SecondBase: " << second << std::endl;

  std::cout << "\nBeginning to call test functions: " << std::endl;


  Diamond d;
  d.testFunction1();

  DiamondCrown dc;
  dc.testFunction2();

  base->testFunction1();
  base->testFunction2();

  derived->testFunction1();
  derived->testFunction2();
}
