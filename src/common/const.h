#ifndef CONST_H
#define CONST_H

#include <cmath>
#include <cstdlib>

typedef unsigned long long uint64;

namespace Const
{
constexpr int HASH_BASE = 31415927;

constexpr double EPS = 1e-6;
constexpr double PI = acos(-1.0);

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

inline double randDouble()
{
#ifdef __linux
    return 1.0 * rand() / RAND_MAX;
#else
    return 1.0 * randUInt() / (1 << 20);
#endif
}
}

#endif // CONST_H
