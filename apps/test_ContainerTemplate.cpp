
#include <memory>
#include <iostream>

class Object
{
public:

  std::string mData;

  Object(const std::string& data = "")
    : mData(data)
  {
    // Do nothing
  }

  template<template <typename> class Container>
  Container<Object> clone()
  {
    return Container<Object>(new Object(*this));
  }

  template<template <typename...> class Container, typename... Args>
  Container<Object, Args...> clone(Args&&... args)
  {
    return Container<Object, Args...>(new Object(*this),
                                      std::forward<Args>(args)...);
  }

  std::shared_ptr<Object> clone()
  {
    return clone<std::shared_ptr>();
  }
};

void customDeleter(Object* object)
{
  std::cout << "Successfully using a custom deleter! Data was: "
            << object->mData << std::endl;
  delete object;
}

int main()
{
  Object original("This test worked");
  std::unique_ptr<Object> unique_test = original.clone<std::unique_ptr>();

  std::shared_ptr<Object> shared_test = original.clone();

  auto custom_delete_test =
      unique_test->clone<std::unique_ptr, void (*)(Object*)>(&customDeleter);

  std::cout << original.mData << "\n"
            << unique_test->mData << "\n"
            << shared_test->mData << "\n"
            << custom_delete_test->mData << std::endl;
}
