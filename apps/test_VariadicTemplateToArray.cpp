
#include <array>
#include <vector>
#include <iostream>
#include <typeinfo>

namespace derp {

class Base1
{
public:

  static constexpr const char* InterfaceName = "Base1";

};

class Base2
{
public:

  static constexpr const char* InterfaceName = "Base2";

  struct SubBase2
  {

  };

};

class Base3
{
public:

  static constexpr const char* InterfaceName = "Base3";

};

} // namespace derp

using namespace derp;

template<typename... Bases>
class Joiner
{
public:

  constexpr static std::array<const char*, sizeof...(Bases)> InterfaceNames =
  { Bases::InterfaceName... };

};

struct InGlobalNamespace
{

};

void someFunction()
{
  static_assert(std::is_same<Base2::SubBase2, ::Base2::SubBase2>::value,
                "Base2 must be in global namespace");
}

class TestClass : public Joiner<Base1, Base2, Base3>
{
public:

};


int main()
{
  const std::array<const char*, TestClass::InterfaceNames.size()> names =
      TestClass::InterfaceNames;

  std::cout << "Array names:\n";
  for(std::size_t i=0; i < names.size(); ++i)
    std::cout << names[i] << std::endl;

  std::vector<std::string> v_names;
//  v_names.reserve(TestClass::InterfaceNames.size());
  for(const char* name : names)
    v_names.push_back(name);

  std::cout << "\nVector names:\n";
  for(std::size_t i=0; i < v_names.size(); ++i)
    std::cout << v_names[i] << std::endl;

  static_assert(std::is_same<InGlobalNamespace, ::InGlobalNamespace>::value,
                "Class must be in global namespace");
}

