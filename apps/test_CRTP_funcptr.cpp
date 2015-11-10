
#include <iostream>
#include <typeinfo>

template <typename T>
void my_func(T)
{
  std::cout << "Calling update on type " << typeid(T).name() << std::endl;
}

template <class BaseT, void (*update)(BaseT) = my_func >
class CRTPClass
{
public:

  using Base = BaseT;
  constexpr static void (*Update)(Base) = update;
  using Implementation = CRTPClass<BaseT, update>;

};

template <size_t K>
class MyTemplatedNest
{
public:

  class MyBase : public CRTPClass<MyBase, my_func<MyBase> >
  {
  public:

    MyBase();
//    {
//      MyTemplatedNest<10>::MyBase::Implementation::Update(*this);
//    }

    void CallUpdate();
//    {
//      Implementation::Update(*this);
//    }

  };

};

template <size_t K>
MyTemplatedNest<K>::MyBase::MyBase()
{
  MyTemplatedNest<K>::MyBase::Update(*this);
}

class MyNest
{
public:

  class MyBase : public CRTPClass<MyBase, my_func<MyBase> >
  {
  public:

    MyBase()
    {
//      CRTPClass<MyBase, my_func<MyBase> >::Update(*this);
      Implementation::Update(*this);
    }

    void CallUpdate()
    {
      Implementation::Update(*this);
    }

  };

};

class MyBase : public CRTPClass<MyBase, my_func<MyBase> >
{
public:

  MyBase()
  {
    Update(*this);
  }

};



int main ()
{
  MyBase base;
  MyNest::MyBase nestedBase;
  MyTemplatedNest<10>::MyBase templateNestedBase;
}
