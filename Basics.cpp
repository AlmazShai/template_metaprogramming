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

template<typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

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