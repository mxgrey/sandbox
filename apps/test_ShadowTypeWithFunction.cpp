
#include <memory>

class SomeType
{

};

using SomeTypePtr = std::shared_ptr<SomeType>;

class OtherType
{
  public:
    SomeTypePtr SomeType()
    {
      return SomeTypePtr();
    }
};

int main()
{
  OtherType other;

  SomeTypePtr some = other.SomeType();
}
