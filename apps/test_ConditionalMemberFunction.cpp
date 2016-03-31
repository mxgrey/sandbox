
#include <cstddef>
#include <type_traits>
#include <iostream>

template <bool B, class Type, typename... Dependencies>
struct deferred_enable_if { };

template<class Type, typename... Dependencies>
struct deferred_enable_if<true, Type, Dependencies...> { typedef Type type; };

template<bool offerFunctions, class Spec>
class Base
{
public:

  template <class T>
  typename std::enable_if<offerFunctions, T>::type getThing()
  {
    return _getThing(type<T>());
  }

  template <class T>
  typename deferred_enable_if<offerFunctions, size_t, T>::type getNumOfThings() const
  {
    // return a number related to type T
  }

  template <class T>
  typename deferred_enable_if<offerFunctions, void, T>::type doAThing(T thing)
  {
    // do something related to type T
  }

protected:

  template <class T> struct type { };

  template <class T>
  typename deferred_enable_if<offerFunctions, Spec, T>::type _getThing(type<Spec>)
  {
    std::cout << "Specialized!" << std::endl;
    return Spec();
  }

  template <class T>
  typename std::enable_if<offerFunctions, T>::type _getThing(type<T>)
  {
    std::cout << "Not Specialized" << std::endl;
    return T();
  }

};

class DerivedOkay : public Base<true, double>
{

};

class DerivedError : public Base<false, double>
{

};

int main()
{
//  DerivedError d;
  DerivedOkay d;
  d.getThing<int>();
  d.getThing<double>();
}
