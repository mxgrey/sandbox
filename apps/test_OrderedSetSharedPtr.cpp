
#include <set>
#include <memory>

int main()
{
  std::set<std::shared_ptr<std::string>> set;

  auto value1 = std::make_shared<std::string>("value 1");
  auto value2 = std::make_shared<std::string>("value 2");
  set.insert(value1);
  std::set<std::shared_ptr<std::string>>::iterator p2iter = set.insert(value2).first;

//  *p2iter = value1; // Does not compile because p2iter is actually a const-iterator
}
