

#include <array>
#include <iostream>




int main()
{
  const std::size_t N = 10;
  std::array<double, N> a{10.0}; // Fills in everything after the first entry with zeros

  for(std::size_t i=0; i < N; ++i)
    std::cout << a[i] << std::endl;
}
