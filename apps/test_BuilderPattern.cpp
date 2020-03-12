
#include <vector>
#include <string>

#include <iostream>

struct FooMessage
{
  std::string text;
  int id;
};

struct BarMessage
{
  int value_1;
  double value_2;
  FooMessage foo;
  std::vector<double> more_values;
};

template<typename> struct build;

struct Init_FooMessage_id
{
  Init_FooMessage_id(FooMessage& msg) : _msg(msg) { }

  FooMessage id(int v)
  {
    _msg.id = v;
    return _msg;
  }

private:
  FooMessage& _msg;
};

template<>
struct build<FooMessage>
{
  Init_FooMessage_id text(std::string v)
  {
    _msg.text = std::move(v);
    return Init_FooMessage_id(_msg);
  }

private:
  FooMessage _msg;
};


struct Init_BarMessage_more_values
{
  Init_BarMessage_more_values(BarMessage& msg) : _msg(msg) { }

  BarMessage more_values(std::vector<double> v)
  {
    _msg.more_values = std::move(v);
    return _msg;
  }

private:
  BarMessage& _msg;
};

struct Init_BarMessage_foo
{
  Init_BarMessage_foo(BarMessage& msg) : _msg(msg) { }

  Init_BarMessage_more_values foo(FooMessage v)
  {
    _msg.foo = std::move(v);
    return Init_BarMessage_more_values(_msg);
  }

private:
  BarMessage& _msg;
};

struct Init_BarMessage_value_2
{
  Init_BarMessage_value_2(BarMessage& msg) : _msg(msg) { }

  Init_BarMessage_foo value_2(double v)
  {
    _msg.value_2 = v;
    return Init_BarMessage_foo(_msg);
  }

private:
  BarMessage& _msg;
};

template<>
struct build<BarMessage>
{

  Init_BarMessage_value_2 value_1(int v)
  {
    _msg.value_1 = v;
    return Init_BarMessage_value_2(_msg);
  }

private:
  BarMessage _msg;
};


int main()
{
  BarMessage bar_msg = build<BarMessage>()
      .value_1(5)
      .value_2(10)
      .foo(build<FooMessage>()
           .text("some text")
           .id(100))
      .more_values({1.0, 2.5, 3.333, 0.4});

  std::cout << "value_1: " << bar_msg.value_1
            << "\nvalue_2: " << bar_msg.value_2
            << "\nfoo.text: " << bar_msg.foo.text
            << "\nfoo.id: " << bar_msg.foo.id
            << "\nmore_values:";

  for (const auto v : bar_msg.more_values)
    std::cout << " " << v;
  std::cout << std::endl;
}
