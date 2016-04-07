
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
  struct StateData
  {
    StateData(int i = 0)
      : val1(i)
    {
      // Do nothing
    }

    int val1;
  };
  struct State : StateData
  {
    using Data = StateData;
    State(const Data& data) : Data(data) { }

    template <typename... Args>
    State(Args&&... args)
      : Data(std::forward<Args>(args)...)
    {
      // Do nothing
    }
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
  struct StateData
  {
    StateData(double d = 0)
      : val2(d)
    {
      // Do nothing
    }

    double val2;
  };
  struct State : StateData
  {
    using Data = StateData;
    State(const Data& data) : Data(data) { }

    template <typename... Args>
    State(Args&&... args)
      : Data(std::forward<Args>(args)...)
    {
      // Do nothing
    }
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
template <class Object>
struct GetState
{
  using Type = typename Object::State;
};

template <class Object>
struct GetProperties
{
  using Type = typename Object::Properties;
};

//==============================================================================
template <template<class> class GetData, typename... Types>
class MakeThing
{
public:

  template <typename... Args>
  MakeThing(Args&&...)
  {
    // Do nothing
  }

  virtual ~MakeThing() = default;
};

template <template<class> class GetData, typename Object,
          typename... Remainder>
class MakeThing<GetData, Object, Remainder...> :
    public GetData<Object>::Type,
    public MakeThing<GetData, Remainder...>
{
public:

  enum Delegate_t { Delegate };

  using Base = typename GetData<Object>::Type;

  template <typename Arg>
  struct ConvertIfData
  {
    using type = typename static_if_else<
        std::is_base_of<typename Base::Data, Arg>::value,
        typename Base::Data, Arg>::type;
  };

  template <typename... Args>
  MakeThing(Args&&... args)
    : MakeThing<GetData, Remainder...>(std::forward<Args>(args)...)
  {
    _findData(std::forward<Args>(args)...);
  }

  void print() const
  {
    std::cout << "\nPrinting " << typeid(Object).name() << " with "
              << typeid(Base).name() << std::endl;
  }

protected:

  void _findData()
  {
    // Do nothing
  }

  template <typename Arg1, typename... Args>
  void _findData(Arg1&& arg1, Args&&... args)
  {
    _useIfData(static_cast<const typename ConvertIfData<Arg1>::type&>(arg1));
    _findData(std::forward<Args>(args)...);
  }

  template <typename Arg>
  void _useIfData(Arg)
  {
    // Do nothing
  }

  void _useIfData(const typename Base::Data& data)
  {
    static_cast<Base&>(*this) = data;
  }

};

//==============================================================================
template <class... Objects>
using MakeState = MakeThing<GetState, Objects...>;

template <class... Objects>
using MakeProperties = MakeThing<GetProperties, Objects...>;

int main()
{
  std::cout << "making state" << std::endl;
  Example1::State e1_state;
  Example1::StateData e1_statedata;
  MakeState<Example1> state(e1_statedata);


  std::cout << "\nmaking otherState" << std::endl;
  MakeState<Example1, Example2> otherState(
        Example1::State(6), Example2::State(4e-2));

  std::cout << "\notherState:\n";
  std::cout << "val1: " << otherState.val1 << std::endl;
  std::cout << "val2: " << otherState.val2 << std::endl;

  MakeState<Example2, Example1> switchedState(
        Example1::State(6), Example2::State(4e-2));

  std::cout << "\nswitchedState:\n";
  std::cout << "val1: " << switchedState.val1 << std::endl;
  std::cout << "val2: " << switchedState.val2 << std::endl;

}
