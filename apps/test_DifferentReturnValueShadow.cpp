
template<typename S>
struct Data
{

};

template<typename S>
class Base
{
public:

  virtual Data<S> getData() = 0;

};

class Derived1 : public Base<float>
{
public:

  Data<float> getData() override
  {
    return Data<float>();
  }

};

class Derived2 : public Base<double>
{
public:

  Data<double> getData() override
  {
    return Data<double>();
  }

};


int main()
{
  Derived1 d1;
}
