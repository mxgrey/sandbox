
#include <stdlib.h>

#include <iostream>

int main()
{
  // This program demonstrates that using setenv does not have a lasting impact
  // on the environment outside of this executable. It seems that a copy of the
  // shell environment is made and provided to this program; then this program
  // may modify the environment variables however it pleases without an impact
  // on the shell environment that launched it.

  char* initial_val = getenv("SOME_VARIABLE");
  if(initial_val)
    std::cout << "SOME_VARIABLE:" << initial_val << std::endl;
  else
    std::cout << "No initial value" << std::endl;

  setenv("SOME_VARIABLE", "here_is_a_value", 1);
  std::cout << "SOME_VARIABLE:" << getenv("SOME_VARIABLE") << std::endl;
}
