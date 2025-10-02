#pragma once

#include <tuple>
#include <type_traits>

namespace TMP {

template <typename T>
struct Reverse
{
};

template <typename Head>
struct Reverse<std::tuple<Head>>
{
    using type = std::tuple<Head>;
};

template <typename Head, typename... Tail>
struct Reverse<std::tuple<Head, Tail...>>
{
    using rest = typename Reverse<std::tuple<Tail...>>::type;
    using type = decltype(std::tuple_cat(std::declval<rest>(), std::declval<std::tuple<Head>>()));
};

static_assert(std::is_same_v<std::tuple<int, bool, float>, typename Reverse<std::tuple<float, bool, int>>::type>);
static_assert(std::is_same_v<std::tuple<int, int, double>, typename Reverse<std::tuple<double, int, int>>::type>);
}