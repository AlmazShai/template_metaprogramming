#pragma once

#include <type_traits>

namespace TMP {

template <int... Args>
struct Vector
{
};

template <typename Vec, typename Res = Vector<>>
struct Unique
{
};

// Empty input
template <int... Out>
struct Unique<Vector<>, Vector<Out...>>
{
    using type = Vector<Out...>;
};

// Base case
template <int Last, int... Out>
struct Unique<Vector<Last>, Vector<Out...>>
{
    using type = Vector<Out..., Last>;
};

// Head matches with prev value
template <int Prev, int... Rest, int... Out>
struct Unique<Vector<Prev, Prev, Rest...>, Vector<Out...>>
{
    using type = typename Unique<Vector<Prev, Rest...>, Vector<Out...>>::type;
};

// No match
template <int Prev, int Head, int... Rest, int... Out>
struct Unique<Vector<Prev, Head, Rest...>, Vector<Out...>>
{
    using type = typename Unique<Vector<Head, Rest...>, Vector<Out..., Prev>>::type;
};

static_assert(std::is_same_v<Unique<Vector<1, 1, 2, 3, 3>>::type, Vector<1, 2, 3>>);
static_assert(std::is_same_v<Unique<Vector<1, 2, 2, 2, 3>>::type, Vector<1, 2, 3>>);
static_assert(std::is_same_v<Unique<Vector<>>::type, Vector<>>);

}