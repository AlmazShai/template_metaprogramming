#pragma once


template<int Val1, int Val2>
struct GCD;

template<int Val1>
struct GCD<Val1, 0> {
    static constexpr int value = Val1;
};

template<int Val1, int Val2>
struct GCD {
    static constexpr int value = GCD<Val2, Val1 % Val2>::value;
};



static_assert(GCD<6, 3>::value == 3);
static_assert(GCD<16, 12>::value == 4);
static_assert(GCD<35, 15>::value == 5);
static_assert(GCD<3, 6>::value == 3);
static_assert(GCD<1, 1>::value == 1);
static_assert(GCD<3, 0>::value == 3);
static_assert(GCD<0, 3>::value == 3);