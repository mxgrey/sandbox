
#include <utility>
#include <iostream>
#include <typeindex>

template <bool B, typename T_if, typename T_else>
struct static_if_else
{
  using type = T_else;
};

template <typename T_if, typename T_else>
struct static_if_else<true, T_if, T_else>
{
  using type = T_if;
};



//==============================================================================
class Example1
{
public:
  struct StateData { };
  struct State : StateData
  {
    using Data = StateData;
    State(const Data& data) : Data(data) { }

    State(int i = 0)
      : val(i)
    {
      // Do nothing
    }

    int val;
  };

  struct PropertiesData { };
  struct Properties : PropertiesData
  {
    using Data = PropertiesData;
    Properties() = default;
    Properties(const Data& data) : Data(data) { }
  };
};

//==============================================================================
class Example2
{
public:
  struct StateData { };
  struct State : StateData
  {
    using Data = StateData;
    State() = default;
    State(const Data& data) : Data(data) { }
  };

  struct PropertiesData { };
  struct Properties : PropertiesData
  {
    using Data = PropertiesData;
    Properties() = default;
    Properties(const Data& data) : Data(data) { }
  };
};

//==============================================================================
class Example3
{
public:
  struct StateData { };
  struct State : StateData
  {
    using Data = StateData;
    State() = default;
    State(const Data& data) : Data(data) { }
  };

  struct PropertiesData { };
  struct Properties : PropertiesData
  {
    using Data = PropertiesData;
    Properties() = default;
    Properties(const Data& data) : Data(data) { }
  };
};

//==============================================================================
template <typename... Types>
class MakeThing
{
public:

  MakeThing()
  {
    std::cout << "Reached terminal class" << std::endl;
  }

  virtual ~MakeThing() = default;
};

template <class Object, typename T, typename... Remainder>
class MakeThing<Object, T, Remainder...> :
    public T,
    public MakeThing<Remainder...>
{
public:

  enum Delegate_t { Delegate };

  template <typename Arg>
  struct ConvertIfData
  {
    using type = typename static_if_else<
        std::is_base_of<typename T::Data, Arg>::value,
        typename T::Data, Arg>::type;
  };

  MakeThing()
  {
    std::cout << "Called empty constructor for " << typeid(Object).name()
              << std::endl;
  }

  template <typename Arg1, typename... RemainingArgs>
  MakeThing(const Arg1& arg1, RemainingArgs&&... args)
    : MakeThing(Delegate,
                static_cast<const typename ConvertIfData<Arg1>::type&>(arg1),
                std::forward<RemainingArgs>(args)...)
  {
    std::cout << "Calling delegating constructor" << std::endl;
  }

  void print() const
  {
    std::cout << "\nPrinting " << typeid(Object).name() << " with "
              << typeid(T).name() << std::endl;
  }

//  MakeThing(T&& thing)
//    : T(std::move(thing))
//  {
//    std::cout << "using move constructor for " << typeid(Object).name()
//              << " with " << typeid(T).name() << std::endl;
//  }

protected:

  template <typename... RemainingArgs>
  MakeThing(
      Delegate_t, const typename T::Data& data, RemainingArgs&&... args)
    : T(data),
      MakeThing<Remainder...>(std::forward<RemainingArgs>(args)...)
  {
    std::cout << "Make " << typeid(Object).name() << " with "
              << typeid(typename T::Data).name() << std::endl;
  }

  template <typename... RemainingArgs>
  MakeThing(Delegate_t, RemainingArgs&&... args)
    : T(),
      MakeThing<Remainder...>(std::forward<RemainingArgs>(args)...)
  {
    std::cout << "Make " << typeid(Object).name() << " empty" << std::endl;
  }

};

//==============================================================================
template <class Object>
using MakeState = MakeThing<Object, typename Object::State>;

template <class Object>
using MakeProperties = MakeThing<Object, typename Object::Properties>;

int main()
{
  using State = Example1::State;
  std::cout << "making state" << std::endl;
  Example1::State e1_state;
  Example1::StateData e1_statedata;
  MakeState<Example1> state(e1_statedata);

  std::cout << "\nmaking otherState" << std::endl;
//  MakeState<Example1> otherState(Example1::State());
  MakeState<Example1> otherState(State(0));

  otherState.print();
}
