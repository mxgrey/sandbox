
#include <optional>
#include <string>
#include <iostream>

struct SomeClass
{
  SomeClass(const std::string& v) : value(v) { }

  std::string value;
};

int main()
{
  std::optional<const SomeClass> osc = std::make_optional<SomeClass>("v");

  SomeClass copy = *osc;

  std::cout << osc->value << "\n" << copy.value << std::endl;
}
