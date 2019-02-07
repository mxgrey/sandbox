
#include <algorithm>
#include <vector>
#include <limits>
#include <iostream>

namespace custom {

template <class T>
class allocator
{
public:
    using value_type    = T;

     using pointer       = value_type*;
     using const_pointer = typename std::pointer_traits<pointer>::template
                                                     rebind<value_type const>;
     using void_pointer       = typename std::pointer_traits<pointer>::template
                                                           rebind<void>;
     using const_void_pointer = typename std::pointer_traits<pointer>::template
                                                           rebind<const void>;

     using difference_type = typename std::pointer_traits<pointer>::difference_type;
     using size_type       = std::make_unsigned_t<difference_type>;

     template <class U> struct rebind {typedef allocator<U> other;};

    allocator() noexcept {}  // not required, unless used
    template <class U> allocator(allocator<U> const&) noexcept {}

    value_type*  // Use pointer if pointer is not a value_type*
    allocate(std::size_t n)
    {
        return static_cast<value_type*>(::operator new (n*sizeof(value_type)));
    }

    void
    deallocate(value_type* p, std::size_t) noexcept  // Use pointer if pointer is not a value_type*
    {
        ::operator delete(p);
    }

     value_type*
     allocate(std::size_t n, const_void_pointer)
     {
         return allocate(n);
     }

     template <class U, class ...Args>
     void
     construct(U* p, Args&& ...args)
     {
         ::new(p) U(std::forward<Args>(args)...);
     }

     template <class U>
     void
     destroy(U* p) noexcept
     {
         p->~U();
     }

     std::size_t
     max_size() const noexcept
     {
         return std::numeric_limits<size_type>::max();
     }

     allocator
     select_on_container_copy_construction() const
     {
         return *this;
     }

     using propagate_on_container_copy_assignment = std::false_type;
     using propagate_on_container_move_assignment = std::false_type;
     using propagate_on_container_swap            = std::false_type;
     using is_always_equal                        = std::is_empty<allocator>;
};

template <class T, class U>
bool
operator==(allocator<T> const&, allocator<U> const&) noexcept
{
    return true;
}

template <class T, class U>
bool
operator!=(allocator<T> const& x, allocator<U> const& y) noexcept
{
    return !(x == y);
}

} // namespace custom


int main()
{
  std::vector<double> default_vector = {0.0, 0.5, 1.0, 1.5, 2.0};
  std::vector<double, custom::allocator<double>> custom_vector;

  const std::size_t max = 4;
  const auto N = std::min(default_vector.size(), max);

  custom_vector.resize(N);

  std::copy(default_vector.begin(), default_vector.begin()+static_cast<long long int>(N), custom_vector.begin());

  for(const auto& v : custom_vector)
    std::cout << v << ", ";
  std::cout << std::endl;
}
