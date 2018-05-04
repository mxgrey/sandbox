

#include <iostream>


int main()
{
  std::size_t iters = 0;

  double last = -10.0;
  double current = last;
  do
  {
    current *= 10.0;

    last = current;

    current += 0.25;

    std::cout << iters << std::endl;
    ++iters;
    if(iters > 10000000)
      break;

  } while(current != last);

  std::cout << "final value: " << current << std::endl;
}
