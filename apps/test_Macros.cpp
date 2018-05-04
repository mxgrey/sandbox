
#include <iostream>

#define STRINGIFY(x) #x

#define WARN_SUP_MSVC_HELPER(w) STRINGIFY(warning(disable: w))

class Comparator
{
public:

  Comparator()
    : val(0)
  {
    // Do nothing
  }

  #define IMPLEMENT_OPERATOR( op ) inline bool operator op (const Comparator& other) const { return (val op other.val); }

  IMPLEMENT_OPERATOR(<)
  IMPLEMENT_OPERATOR(==)
  IMPLEMENT_OPERATOR(!=)

  double val;
};

int main()
{
  std::cout << WARN_SUP_MSVC_HELPER("4265") << std::endl;

  Comparator c1;
  Comparator c2;
  std::cout << "Equal: " << (c1 == c2) << std::endl;
  std::cout << "Not equal: " << (c1 != c2) << std::endl;
  std::cout << "Less than: " << (c1 < c2) << std::endl;
}
