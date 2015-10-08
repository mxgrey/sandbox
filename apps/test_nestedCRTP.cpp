
#include <iostream>

namespace detail
{

template <class Base>
void no_op(Base)
{
  std::cout << "Doing nothing!" << std::endl;
}

} // namespace detail



template <class Base, class StateData, class PropertiesData,
          void (*updateState)(Base*) = &detail::no_op<Base*>,
          void (*updateProperties)(Base*) = updateState >
class CRTPImplementation
{
public:

  using State = StateData;
  using Properties = PropertiesData;

  void setState(const State& state)
  {
    mState = state;
    (*updateState)(static_cast<Base*>(this));
  }

  void setProperties(const Properties& properties)
  {
    mProperties = properties;
    (*updateProperties)(static_cast<Base*>(this));
  }

protected:

  State mState;
  Properties mProperties;

};


class MyObject;
namespace detail
{

struct MyObjectState
{

};

struct MyObjectProperties
{

};

void MyObjectUpdate(MyObject* /*object*/)
{
  std::cout << "updating MyObject" << std::endl;
}

void MyObjectStateUpdate(MyObject* /*object*/)
{
  std::cout << "updating State" << std::endl;
}

void MyObjectPropertiesUpdate(MyObject* /*object*/)
{
  std::cout << "updating Properties" << std::endl;
}

} // namespace detail

class MyObject : public CRTPImplementation<
    MyObject,
    detail::MyObjectState,
    detail::MyObjectProperties,
    &detail::MyObjectUpdate>

//    &detail::MyObjectStateUpdate,
//    &detail::MyObjectPropertiesUpdate>
{

};

int main()
{
  MyObject instance;
  instance.setState(MyObject::State());
  instance.setProperties(MyObject::Properties());
}
