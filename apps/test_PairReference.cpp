
#include <utility>

class MyType
{

};

using PairReference = std::pair<MyType&, const bool>;

class Container
{
public:

  PairReference getPairReference()
  {
    return PairReference{mObject, true};
  }

private:
  MyType mObject;
};


int main()
{
//  Container container;
//  PairReference pair = container.getPairReference();

//  PairReference copyPair = pair;

//  pair = copyPair;
}
