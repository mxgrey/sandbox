
#include <tuple>
#include <iostream>

#define LC(l) \
  l ## l ## l ## l ## l ## l ## l ## l ## l ## l ## \
  l ## l ## l ## l ## l ## l ## l ## l ## l ## l ## \
  l ## l ## l ## l ## l ## l ## l ## l ## l ## l

#define DECLARE_LETTER_CLASS(letter) \
  struct LC(letter) \
  { \
    void print_ ## letter () \
    { \
      std::cout << #letter << std::endl; \
    } \
  };

namespace scratchpad {
namespace testing {

DECLARE_LETTER_CLASS(a);
DECLARE_LETTER_CLASS(b);
DECLARE_LETTER_CLASS(c);
DECLARE_LETTER_CLASS(d);
DECLARE_LETTER_CLASS(e);
DECLARE_LETTER_CLASS(f);

DECLARE_LETTER_CLASS(g);
DECLARE_LETTER_CLASS(h);
DECLARE_LETTER_CLASS(i);
DECLARE_LETTER_CLASS(j);
DECLARE_LETTER_CLASS(k);
DECLARE_LETTER_CLASS(l);

DECLARE_LETTER_CLASS(m);
DECLARE_LETTER_CLASS(n);
DECLARE_LETTER_CLASS(o);
DECLARE_LETTER_CLASS(p);
DECLARE_LETTER_CLASS(q);
DECLARE_LETTER_CLASS(r);

DECLARE_LETTER_CLASS(s);
DECLARE_LETTER_CLASS(t);
DECLARE_LETTER_CLASS(u);
DECLARE_LETTER_CLASS(v);
DECLARE_LETTER_CLASS(w);
DECLARE_LETTER_CLASS(x);



struct Empty { };

template<typename...>
struct IterateTuple
{
  using next = Empty;
};

template<typename Next, typename... T>
struct IterateTuple<Next, T...>
{
  using v = Next;
  using next = IterateTuple<T...>;
};


template<typename Iterable>
struct GetNext
{
  struct n : Iterable::next { };
//  using n = typename Iterable::next;
};

struct a2f : IterateTuple<LC(a), LC(b), LC(c), LC(d), LC(e), LC(f)> { };
struct g2l : IterateTuple<LC(g), LC(h), LC(i), LC(j), LC(k), LC(l)> { };
struct m2r : IterateTuple<LC(m), LC(n), LC(o), LC(p), LC(q), LC(r)> { };
struct s2x : IterateTuple<LC(s), LC(t), LC(u), LC(v), LC(w), LC(x)> { };
struct m2x : IterateTuple<LC(m), LC(n), LC(o), LC(p), LC(q), LC(r), LC(s), LC(t), LC(u), LC(v), LC(w), LC(x)> { };
//struct Named : IterateTuple<a2f, g2l, m2x> { };
struct Named : IterateTuple<a2f, g2l, m2r, s2x> { };

//struct Named : IterateTuple<
//    LC(a), LC(b), LC(c), LC(d), LC(e), LC(f),
//    LC(g), LC(h), LC(i), LC(j), LC(k), LC(l),
//    LC(m), LC(n), LC(o), LC(p), LC(q), LC(r),
//    LC(s), LC(t), LC(u), LC(v), LC(w), LC(x)> { };

template<typename Input, typename = std::void_t<>>
struct ValueOrTuple
{
  using type = Input;
};

template<typename Bases, typename = std::void_t<>>
struct Aggregate
{
  struct type { };
};

template<typename Bases>
struct Aggregate<Bases, std::void_t<typename Bases::v>>
{
  struct type :
      ValueOrTuple<typename Bases::v>::type,
      Aggregate<typename GetNext<Bases>::n>::type { };
};

template<typename Input>
struct ValueOrTuple<Input, std::void_t<typename Input::v>>
{
  using type = typename Aggregate<Input>::type;
};

}
}

using namespace scratchpad::testing;

int main()
{
  using Current0 = a2f;
//  std::cout << typeid(Current0).name() << std::endl;
  std::cout << typeid(Current0::v).name() << std::endl;
  std::cout << "\n";

  using Current1 = GetNext<Current0>::n;
//  std::cout << typeid(Current1).name() << std::endl;
  std::cout << typeid(Current1::v).name() << std::endl;
  std::cout << "\n";

  using Current2 = GetNext<Current1>::n;
//  std::cout << typeid(Current2).name() << std::endl;
  std::cout << typeid(Current2::v).name() << std::endl;
  std::cout << "\n";

  using Current3 = GetNext<Current2>::n;
//  std::cout << typeid(Current3).name() << std::endl;
//  std::cout << typeid(Current3::v).name() << std::endl;
  std::cout << "\n";

  Aggregate<Named>::type aggregate;
  aggregate.print_a();
  aggregate.print_b();
  aggregate.print_c();
  aggregate.print_d();
  aggregate.print_e();
  aggregate.print_f();
  aggregate.print_g();
  aggregate.print_h();
  aggregate.print_i();
  aggregate.print_j();
  aggregate.print_k();
  aggregate.print_l();
  aggregate.print_m();
  aggregate.print_n();
  aggregate.print_o();
  aggregate.print_p();
  aggregate.print_q();
  aggregate.print_r();
  aggregate.print_s();
  aggregate.print_t();
  aggregate.print_u();
  aggregate.print_v();
  aggregate.print_w();
  aggregate.print_x();

  using Current4 = GetNext<Current3>::n;
  std::cout << typeid(Aggregate<Named>::type).name() << std::endl;
//  std::cout << typeid(Current4).name() << std::endl;
//  std::cout << typeid(Current4::v).name() << std::endl;

//  using Current5 = GetNext<Current4>::n;
//  std::cout << typeid(Current5).name() << std::endl;
}
