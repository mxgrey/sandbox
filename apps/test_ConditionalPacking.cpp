
#include <utility>

class Composite;

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

struct AspectBase
{
  template <typename T>
  struct Aspectify : public T
  {
    Aspectify() = default;

    Aspectify(const T& data)
    {
      static_cast<T&>(*this) = data;
    }
  };

  AspectBase(Composite* comp)
    : mComposite(comp)
  {
    // Do nothing
  }

  Composite* mComposite;
};

struct SomeStateData { };
struct SomePropData { };

template <class Base = AspectBase>
struct Aspect_state : public Base
{
  using StateData = SomeStateData;
  using State = AspectBase::Aspectify<StateData>;

  enum Delegate_t { Delegate };

  Aspect_state(Composite* comp)
    : Base(comp)
  {
    // Do nothing
  }

  template <typename... Args>
  Aspect_state(Delegate_t, Composite* comp, Args&&... args)
    : Base(comp, args...)
  {
    // Do nothing
  }

  template <typename T>
  struct ConvertIfState
  {
    using type = typename static_if_else<std::is_base_of<StateData, T>::value, StateData, T>::type;
  };

  template <typename T, typename... Args>
  Aspect_state(Composite* comp, const T& firstArg, Args&&... args)
    : Aspect_state(Delegate, comp,
                   static_cast<const typename ConvertIfState<T>::type&>(firstArg),
                   args...)
  {
    // Do nothing
  }

  template <typename... Args>
  Aspect_state(Delegate_t, Composite* comp, const StateData& state, Args&&... args)
    : Base(comp, args...),
      mState(state)
  {
    // Do nothing
  }

  State mState;
};

template <class Base = AspectBase>
struct Aspect_prop : public Base
{
  using PropData = SomePropData;
  using Prop = AspectBase::Aspectify<SomePropData>;

  enum Delegate_t { Delegate };

  Aspect_prop(Composite* comp)
    : Base(comp)
  {
    // Do nothing
  }

  template <typename... Args>
  Aspect_prop(Delegate_t, Composite* comp, Args&&... args)
    : Base(comp, args...)
  {
    // Do nothing
  }

  template <typename T>
  struct ConvertIfProp
  {
    using type = typename static_if_else<std::is_base_of<PropData, T>::value, PropData, T>::type;
  };

  template <typename T, typename... Args>
  Aspect_prop(Composite* comp, const T& firstArg, Args&&... args)
    : Aspect_prop(Delegate, comp,
                  static_cast<const typename ConvertIfProp<T>::type&>(firstArg),
                  args...)
  {
    // Do nothing
  }

  template <typename... Args>
  Aspect_prop(Delegate_t, Composite* comp, const PropData& prop, Args&&... args)
    : Base(comp, args...),
      mProp(prop)
  {
    // Do nothing
  }


//  template <typename... Args>
//  Aspect_prop(Composite* comp, Args&&... args)
//    : Base(comp, args...)
//  {
//    // Do nothing
//  }

//  template <typename T>
//  struct ConvertIfProp
//  {
//    using type = typename static_if_else<std::is_base_of<PropData, T>::value, PropData, T>::type;
//  };

//  template <typename T, typename... Args>
//  Aspect_prop(Composite* comp, const T& firstArg, Args&&... args)
//    : Aspect_prop(comp,
//                  static_cast<const typename ConvertIfProp<T>::type&>(firstArg),
//                  args...)
//  {
//    // Do nothing
//  }

//  template <typename... Args>
//  Aspect_prop(Composite* comp, const PropData& prop, Args&&... args)
//    : Base(comp, args...),
//      mProp(prop)
//  {
//    // Do nothing
//  }

  Prop mProp;
};

struct Aspect_state_prop : public Aspect_prop< Aspect_state<AspectBase> >
{
  using Base = Aspect_prop< Aspect_state<AspectBase> >;

  Aspect_state_prop(Composite* comp)
    : Base(comp)
  {
    // Do nothing
  }

  Aspect_state_prop(Composite* comp, const PropData& prop)
    : Base(comp, prop)
  {
    // Do nothing
  }

  Aspect_state_prop(Composite* comp, const StateData& state)
    : Base(comp, state)
  {
    // Do nothing
  }

  Aspect_state_prop(Composite* comp,
                    const PropData& prop,
                    const StateData& state)
    : Base(comp, prop, state)
  {
    // Do nothing
  }

  Aspect_state_prop(Composite* comp,
                    const StateData& state,
                    const PropData& prop)
    : Base(comp, prop, state)
  {
    // Do nothing
  }

};

class Composite { };

int main()
{
  SomeStateData state;
  const SomePropData prop;
  Composite* comp = nullptr;

  Aspect_prop<> p_aspect1(comp, prop);
  Aspect_prop<> p_aspect2(comp);

  Aspect_state<> s_aspect1(comp, state);
  Aspect_state<> s_aspect2(comp);

  Aspect_state_prop aspect_1(comp, prop, state);
  Aspect_state_prop aspect_2(comp, state, prop);
  Aspect_state_prop aspect_3(comp, prop);
  Aspect_state_prop aspect_4(comp, state);
  Aspect_state_prop aspect_5(comp);
}











