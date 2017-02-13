
#include <map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <iostream>

//==============================================================================
class Aspect
{

};

namespace detail {

//==============================================================================
struct JointAspect : public Aspect
{
  JointAspect() : val(0) { }

  double val;
};

//==============================================================================
struct SingleDofJointAspect : public Aspect
{

};

//==============================================================================
struct RevoluteJointAspect : public Aspect
{
  RevoluteJointAspect() : R(0) { }

  double R;
};

} // namespace detail


//==============================================================================
class Joint
{
public:

  using Aspect = detail::JointAspect;
};

//==============================================================================
class SingleDofJoint : public Joint
{
public:

  using Aspect = detail::SingleDofJointAspect;
};

//==============================================================================
class RevoluteJoint : public SingleDofJoint
{
public:

  using Aspect = detail::RevoluteJointAspect;
};

//==============================================================================
class Retriever
{
public:

  using AspectMap = std::map< std::type_index, std::unique_ptr<Aspect> >;

  template <class T>
  struct arg { };

  template <bool B, class T_if, class T_else>
  struct static_if_else  { using type = T_if; };

  template <class T_if, class T_else>
  struct static_if_else<false, T_if, T_else> { using type = T_else; };

  template <class T>
  struct AspectOf
  {
    using type = typename static_if_else<std::is_base_of<Aspect, T>::value, T, typename T::Aspect>::type;
  };

  template <class T>
  typename AspectOf<T>::type* get()
  {
    return _get<typename AspectOf<T>::type>();
  }

  template <class T>
  void set(std::unique_ptr<typename AspectOf<T>::type>&& aspect)
  {
    _set(std::move(aspect));
  }

protected:

//  template <class T>
//  typename std::enable_if<std::is_base_of<Aspect, T>::value, T*>::type _get()
//  {
//    AspectMap::iterator it = mAspectMap.find( typeid(T) );
//    if(mAspectMap.end() == it)
//      return nullptr;

//    return static_cast<T*>(it->second.get());
//  }

  template <class T>
  T* _get()
  {
    AspectMap::iterator it = mAspectMap.find( typeid(T) );
    if(mAspectMap.end() == it)
      return nullptr;

    return static_cast<T*>(it->second.get());
  }

  template <class T>
  void _set(std::unique_ptr<T>&& aspect)
  {
    mAspectMap[ typeid(T) ] = std::move(aspect);
  }

//  template <class T>
//  T* _get(arg<T>)
//  {
//    static_assert(false, "The template argument T must be an Aspect or have an Aspect named T::Aspect");

//    return nullptr;
//  }

  AspectMap mAspectMap;

};


int main()
{
  Retriever R;
  R.set<Joint::Aspect>(std::unique_ptr<Joint::Aspect>(new Joint::Aspect()));
  std::cout << "R.get<Joint::Aspect>()->val: "
            <<  R.get<Joint::Aspect>()->val << std::endl;

  R.get<Joint::Aspect>()->val = 3.0;
  std::cout << "\nSet to 3" << std::endl;
  std::cout << "R.get<Joint::Aspect>()->val: "
            <<  R.get<Joint::Aspect>()->val << std::endl;

  std::cout << "R.get<Joint>()->val: "
            <<  R.get<Joint>()->val << std::endl;

  R.set<Joint>(std::unique_ptr<Joint::Aspect>(new Joint::Aspect()));
  std::cout << "\nReset to 0" << std::endl;

  std::cout << "R.get<Joint::Aspect>()->val: "
            <<  R.get<Joint::Aspect>()->val << std::endl;

  std::cout << "R.get<Joint>()->val: "
            <<  R.get<Joint>()->val << std::endl;

  R.get<RevoluteJoint>();
}




















