
#include <iostream>


namespace detail {

//template<typename T>
//struct IsConst : std::integral_constant<bool, false> { };

//template<typename T>
//struct IsConst<const T> : std::integral_constant<bool, true> { };

template<typename To, typename From>
struct ConstCompatible : std::true_type { };

template<typename To, typename From>
struct ConstCompatible<To, const From> : std::integral_constant<bool, std::is_const<To>::value> { };

} // namespace detail

template <typename To, typename From>
using ConstCompatible = detail::ConstCompatible<To, From>;

template <typename T>
class CustomPtr
{
public:

  CustomPtr()
  {
    std::cout << "Calling default constructor" << std::endl;
  }

  template <typename U>
  CustomPtr(const CustomPtr<U>&)
  {
    std::cout << "Calling templated copy constructor" << std::endl;

    static_assert(ConstCompatible<T,U>::value,
                  "Cannot cast from a const-type to a non-const type");
//    if(detail::ConstCompatible<T,U>::value)
//    {
//      std::cout << "Const test passed" << std::endl;
//    }
//    else
//    {
//      std::cout << "Const test FAILED" << std::endl;
//    }
  }

  void print()
  {
    std::cout << "printing message" << std::endl;
  }

};

int main()
{
  {
    std::cout << "Okay:" << std::endl;
    CustomPtr<double> p1{CustomPtr<int>()};
    p1.print();

    std::cout << "Okay:" << std::endl;
    CustomPtr<const double> p2{CustomPtr<int>()};
    p2.print();

//    std::cout << "Bad:" << std::endl;
//    CustomPtr<double> p3{CustomPtr<const int>()};
//    p3.print();
  }

  {
    std::cout << "Okay:" << std::endl;
    CustomPtr<double> p1{CustomPtr<double>()};
    p1.print();

    std::cout << "Okay:" << std::endl;
    CustomPtr<const double> p2{CustomPtr<double>()};
    p2.print();

//    std::cout << "Bad:" << std::endl;
//    CustomPtr<double> p3{CustomPtr<const double>()};
//    p3.print();
  }
}
