
#include <iostream>
#include <limits>
#include <cmath>

int main()
{
  std::cout << static_cast<uint32_t>(std::pow(2, 31)) << std::endl;
  std::cout << std::numeric_limits<int64_t>::max() << std::endl;
  std::cout << std::numeric_limits<std::make_signed_t<uint64_t>>::max() << std::endl;
}
