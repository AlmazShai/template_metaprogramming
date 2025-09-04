// https://www.slamecka.cz/posts/2021-12-05-cpp-metaprogramming-exercises-2/
//
// A set of exercises for type level programming in C++ suitable for programmers with
// intermediate knowledge of template programming and algorithm problems.
//
// Scope:
// - Standard template metaprogramming techniques of moderate difficulty.
// - Focused on algorithm problems.
//
// How to solve it:
// - Make sure you understand topics from Set 1.
// - You should be able to compile with gcc version 11.2, g++ --std=c++20 typelevel_set2.cpp
// - Tests are commented-out and you should uncomment them as you go.
// - You might want to solve the problems on value level first.

#include <algorithm>
#include <iostream>
#include <type_traits>

namespace {

/**
 * 1. Define Fibonacci to contain the n-th number in the series 0,1,1,2,3,5,8,13,...
 *
 * Try three implementations:
 * - Using templated structs (hint: use the SFINAE Enable trick).
 * - Recursive constexpr function.
 * - Iterative constexpr function.
 *
 * Measure the compilation time of each implementation. Observe how the templated-structs implementation
 * is a direct implementation of the recursive definition of Fibonacci sequence, yet it's as fast as the
 * iterative implementation thanks to memoization built into the template system.
 */

// Your code goes here:
// ^ Your code goes here
// template<int N, typename Enable = void>
// struct Fibonacci {
//     static constexpr long unsigned int value = 0;
// };

// template<int N>
// struct Fibonacci<N, std::enable_if_t<N == 1>> {
//     static constexpr long unsigned int value = 1;
// };

// template<int N>
// struct Fibonacci<N, std::enable_if_t<(N > 1)>> {
//     static constexpr long unsigned int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
// };


static constexpr long unsigned int fib(int N) {
    if (N == 0) return 0;
    if (N == 1) return 1;
    return fib(N - 2) + fib(N - 1);
}

// template<int N>
// struct Fibonacci {
//     static constexpr long unsigned int value = fib(N);
// };

//         v
// 0 1 1 2 3 5
// prev: 1
// sum: 2
static constexpr long unsigned int fib2(int N) {
    if (N == 0) return 0;
    if (N == 1) return 1;
    long unsigned int prev = 0;
    long unsigned int sum = 1;
    for(int i = 2; i <= N; i++) {
        auto tmpPrev = sum;
        sum += prev;
        prev = tmpPrev;
    }
    return sum;
}

template<int N>
struct Fibonacci {
    static constexpr long unsigned int value = fib2(N);
};


static_assert(Fibonacci<0>::value == 0);
static_assert(Fibonacci<1>::value == 1);
static_assert(Fibonacci<2>::value == 1);
static_assert(Fibonacci<3>::value == 2);
static_assert(Fibonacci<4>::value == 3);
static_assert(Fibonacci<5>::value == 5);
static_assert(Fibonacci<6>::value == 8);
static_assert(Fibonacci<7>::value == 13);
static_assert(Fibonacci<41>::value % 2 == 1); // Useful for benchmarking, make sure your compiler calculates LHS.


/**
 * 2. Define Concat that takes two vectors and concatenates them into one.
 *
 * This is a warmup before the next exercise.
 */

template<int...>
struct Vector
{};

// Your code goes here:
// ^ Your code goes here
template<typename Vec1, typename Vec2>
struct Concat {
};

template<int...Vals1, int...Vals2>
struct Concat<Vector<Vals1...>, Vector<Vals2...>> {
using type = Vector<Vals1...,Vals2...>;
};


static_assert(std::is_same_v<Concat<Vector<1,2>, Vector<3,4>>::type, Vector<1,2,3,4>>);


/**
 * 3. Run Length Encoding
 */

// Your code goes here:
// ^ Your code goes here
template<int...Vals>
struct RLE;

template<>
struct RLE<> {
    using type = Vector<>;
};

template<int...Args>
struct RLE {
    template<int Count, int...Vals>
    struct Helper;

    // Different than previous
    template<int Count, int Prev, int H, int...Vals>
    struct Helper<Count, Prev, H, Vals...> {
        using type = typename Concat<Vector<Count, Prev>, typename Helper<1, H, Vals...>::type>::type;
    };

    // Match with previous
    template<int Count, int Prev, int...Vals>
    struct Helper<Count, Prev, Prev, Vals...> {
        using type = typename Helper<Count + 1, Prev, Vals...>::type;
    };

    // Last element
    template<int Count, int Last>
    struct Helper<Count, Last> {
        using type = Vector<Count, Last>;
    };
    using type = Helper<1, Args...>::type;
};

static_assert(std::is_same_v<RLE<0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1>::type, Vector<5,0,2,1,12,0,1,1>>);
static_assert(std::is_same_v<RLE<1,1,1,1,9,9,9,2>::type, Vector<4,1,3,9,1,2>>);


/**
 * 4. Minimum Subset Sum Difference
 *    Given a set of numbers, consider partitioning them into two sets, summing each to receive two numbers.
 *    Find the absolute difference between these two numbers that is the smallest (over all possible partitions).
 */

// Your code goes here:
// ^ Your code goes here

// static_assert(MSSD<>::value == 0);
// static_assert(MSSD<1>::value == 1);
// static_assert(MSSD<1, 1>::value == 0);
// static_assert(MSSD<1, 1, 1>::value == 1);
// static_assert(MSSD<20, 30, 10, 10, 20>::value == 10);
// static_assert(MSSD<5, 20, 25, 10, 10, 20>::value == 0);
// static_assert(MSSD<5, 10, 15, 20, 25, 10, 10, 20>::value == 5);


/**
 * 5. Define type Middle to contain the middle letter of a string with an odd length.
 *
 * We'll need a type level string container (requires C++20).
 * This is a warmup before the next exercise.
 */
template<size_t N>
struct MetaString {
    constexpr MetaString(const char (&s)[N])
    {
        std::copy_n(s, N, value);
    }

    char value[N];
    std::size_t size = N;
};

template<MetaString S>
struct Middle
{
    static constexpr char value = S.value[(S.size - 1)/2];
};

static_assert(Middle<"abc">::value == 'b');
static_assert(Middle<"abcde">::value == 'c');

/**
 * 6. Define DeletesToEqual which calculates the minimum number of letters to delete from two strings to make
 * them equal.
 *
 * (This is a simplification of the well-known edit distance problem.)
 */

 

static_assert(DeletesToEqual<"a", "b">::value == 2);
static_assert(DeletesToEqual<"abc", "bcd">::value == 2);
static_assert(DeletesToEqual<"abcd", "bcd">::value == 1);
static_assert(DeletesToEqual<"abc", "xbz">::value == 4);
static_assert(DeletesToEqual<"ababa", "bb">::value == 3);
static_assert(DeletesToEqual<"chocolatefactory", "charlie">::value == 15);

}

int main()
{
}