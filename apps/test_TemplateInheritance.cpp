
template<typename T>
struct Thing
{

};

template<typename T>
struct Policy
{
  using ThingType = Thing<T>;
};

template<typename T, typename U>
struct Print : Policy<T>
{
  using ThingType = typename Policy<T>::ThingType;
  void print(ThingType t)
  {

  }
};

int main()
{

}
