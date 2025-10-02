#pragma once

#include <type_traits>

namespace tmp {

// Implement tuple. This is a forward declaration.
template <typename... Types>
class Tuple;

// Base case
template <>
class Tuple<>
{
};

template <typename Head, typename... Tail>
class Tuple<Head, Tail...>
{
private:
    Head head;
    Tuple<Tail...> tail;

public:
    Tuple()
    {
    }

    Tuple(const Head& head, const Tuple<Tail...>& tail)
        : head(head)
        , tail(tail)
    {
    }

    Tuple(const Head& head, const Tail&... tail)
        : head(head)
        , tail(tail...)
    {
    }

    template <typename VHead, typename... VTail>
    Tuple(VHead&& head, Tuple<VTail...>&& tail)
        : head(std::forward<VHead>(head))
        , tail(std::forward<Tail>(tail)...)
    {
    }

    const Head& getHead() const
    {
        return head;
    }

    Head& getHead()
    {
        return head;
    }

    const Tuple<Tail...>& getTail() const
    {
        return tail;
    }

    Tuple<Tail...>& getTail()
    {
        return tail;
    }
};

// Implement get
template <unsigned N>
struct TupleGet
{
    template <typename Head, typename... Tail>
    static auto apply(const Tuple<Head, Tail...>& t)
    {
        return TupleGet<N - 1>::apply(t.getTail());
    }
};

// Base case
template <>
struct TupleGet<0>
{
    template <typename Head, typename... Tail>
    static const Head& apply(const Tuple<Head, Tail...>& t)
    {
        return t.getHead();
    }
};

template <unsigned N, typename... Types>
constexpr auto get(const Tuple<Types...>& tuple)
{
    return TupleGet<N>::apply(tuple);
}

Tuple<int, float, double> t(1, 1.0f, 2.0);

const auto val = get<2>(t);

static_assert(std::is_same_v<decltype(get<0>(t)), int>);
static_assert(std::is_same_v<decltype(get<1>(t)), float>);
static_assert(std::is_same_v<decltype(get<2>(t)), double>);

}