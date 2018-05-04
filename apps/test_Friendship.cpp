

class PrivateConstructor
{
  template<typename...> friend class Inheritor;
private:
  PrivateConstructor()
  {

  }
};

template <class... Type>
class Inheritor : public PrivateConstructor
{
  friend class Creator;
  template<typename...> friend class Inheritor;

private:
  Inheritor()
  {
    Inheritor<double> test_friendship;
  }
};


class Creator
{
public:
  Creator()
  {
    Inheritor<int> inheritor;
  }
};


int main()
{
  Creator c;
}
