#include <array>
#include <list>
#include <type_traits>
#include <vector>
 
// Basic if condition
template<bool B, typename T, typename F>
struct if_c {
    using type = F;
};

template<typename T, typename F>
struct if_c<true, T, F> {
    using type = T;
};

// Enable if
template<bool B, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T> {
    using type = T;
};

// Checking if type is std::vector
template<typename T>
struct is_vector : std::false_type {};

template<typename T>
struct is_vector<std::vector<T>> : std::true_type {};

// Checking if type is std::array
template<typename T>
struct is_array : std::false_type {};

template<typename T, size_t N>
struct is_array<std::array<T,N>> : std::true_type {};

// Checking if type is std::vector or std::array
template<typename T, typename = void>
struct is_array_or_vector : std::false_type {};

template<typename T>
struct is_array_or_vector<T, typename enable_if<is_array<T>::value or is_vector<T>::value>::type>
    : std::true_type {};


// is_same type trait
template<typename T1, typename T2>
struct is_same : std::false_type {};

template<typename T>
struct is_same<T, T> : std::true_type {};

template<typename T1, typename T2>
static constexpr auto is_same_v = is_same<T1, T2>::value;

static_assert(is_same<int, int>::value);
static_assert(is_same<int*, int*>::value);
static_assert(not is_same<int, const int>::value);
static_assert(is_same_v<float, float>);


// remove_const type trait
template<typename T>
struct remove_const {
    using type = T;
};

template<typename T>
struct remove_const<const T> {
    using type = T;
};

template<typename T>
using remove_const_t = remove_const<T>::type;

static_assert(is_same_v<remove_const_t<const int>, int>);
static_assert(is_same_v<remove_const_t<const int const*>, int const*>);


// is_polymorphic
namespace details {
template<typename T>
std::true_type test(decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr))));
template<typename T>
std::false_type test(...);
}

// template<typename T>
// struct is_polymorphic : decltype(details::test<T>(nullptr)){};

template<typename T, typename = const volatile void*>
struct is_polymorphic : std::false_type {};

template<typename T>
struct is_polymorphic<T, decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr)))> : std::true_type {};

template<typename T>
static constexpr auto is_polymorphic_v = is_polymorphic<T>::value;

struct A { void foo(){}};
struct B { virtual void foo(){}};
struct C { virtual void foo() = 0;};
struct D : C {};
struct E : C {void foo() override {}};
struct F : A {};


static_assert(is_polymorphic_v<A> == false);
static_assert(is_polymorphic_v<B> == true);
static_assert(is_polymorphic_v<C> == true);
static_assert(is_polymorphic_v<D> == true);
static_assert(is_polymorphic_v<E> == true);
static_assert(is_polymorphic_v<F> == false);

int main() {
    // Basic if condition test
    using Type1 = int;
    using Type2 = char;
    using Res1 = if_c<true, Type1, Type2>::type;
    using Res2 = if_c<false, Type1, Type2>::type;

    static_assert(!std::is_same_v<Res1, Res2>, "");
    static_assert(std::is_same_v<Res1, Type1>, "");
    static_assert(std::is_same_v<Res2, Type2>, "");

    // Enable if test
    using EnableIfResult = enable_if<true, Type1>::type;
    static_assert(std::is_same_v<Res1, EnableIfResult>, "");

    // Vector check test
    static_assert(is_vector<std::vector<int>>::value, "");
    static_assert(!is_vector<int>::value, "");

    // is_array_or_vector test
    static_assert(is_array_or_vector<std::vector<int>>::value, "");
    static_assert(is_array_or_vector<std::array<int, 1>>::value, "");
    static_assert(!is_array_or_vector<std::list<int>>::value, "");

    return 0;
}