
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

namespace space {

template<typename T>
class SomeClass
{

};

namespace nam {

class Huff
{

};

inline namespace ill_one
{

class VersionedClass
{

};

} // inline namespace ill_one


inline namespace ill_two
{

class VersionedClass
{

};

} // inline namespace ill_two


} // namespace nam

} // namespace space

using namespace space;
using namespace nam;

template<typename T>
class Foo
{

};

class Bar
{

};



#define PRINT(x) \
  std::cout << #x << ": " << typeid(x).name() << std::endl;

int main()
{
  PRINT(Huff);
  PRINT(SomeClass<Huff>);
  PRINT(Foo<Huff>);
  PRINT(SomeClass<Foo<Huff>>);
  PRINT(Bar);
  PRINT(SomeClass<Bar>);
  PRINT(Foo<Bar>);
  PRINT(SomeClass<Foo<SomeClass<SomeClass<SomeClass<Foo<Bar>>>>>>);
  PRINT(int);
  PRINT(SomeClass<int>);
  PRINT(ill_one::VersionedClass);


//  using LongName = SomeClass<Foo<SomeClass<SomeClass<SomeClass<Foo<Bar>>>>>>;

  int status;
  char* demangled_cstr = abi::__cxa_demangle(
//        typeid(LongName).name(), nullptr, nullptr, &status);
        typeid(ill_two::VersionedClass).name(), nullptr, nullptr, &status);
  const std::string demangled(demangled_cstr);
  free(demangled_cstr);

  std::cout << "Demangled: " << demangled << std::endl;
}
