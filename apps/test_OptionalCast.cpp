
#include <optional>
#include <iostream>

int main()
{
  std::optional<std::size_t> opt = 6;
  const std::size_t value = 6;

  if (opt == value)
    std::cout << "true!" << std::endl;
  else
    std::cout << "false!" << std::endl;
}
