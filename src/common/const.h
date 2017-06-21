#ifndef CONST_H
#define CONST_H

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

typedef unsigned long long uint64;

namespace Const
{
constexpr int HASH_BASE = 31415927;

constexpr double EPS = 1e-6;
constexpr double PI = acos(-1.0);

inline double randDouble()
{
    return 1.0 * rand() / RAND_MAX;
}

inline int randUInt()
{
#ifdef __linux
    return rand();
#else
    return (rand() << 15) | rand();
#endif
}

inline uint64 randUInt64()
{
#ifdef __linux
    return (rand() << 31) | rand();
#else
    return (((((rand() << 15) | rand()) << 15) | rand()) << 15) | rand();
#endif
}
}

#endif // CONST_H
