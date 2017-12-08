#ifndef CONST_H
#define CONST_H

#include <cmath>
#include <cstdlib>

typedef unsigned long long uint64;

namespace Const
{
constexpr int HASH_BASE = 31415927;

constexpr double EPS = 1e-6;
constexpr double PI = M_PI;

inline int randUInt()
{
#ifdef _WIN32
    return (rand() << 15) | rand();
#else
    return rand();
#endif
}

inline uint64 randUInt64()
{
#ifdef _WIN32
    return (((((1ll * rand() << 15) | rand()) << 15) | rand()) << 15) | rand();
#else
    return (1ll * rand() << 31) | rand();
#endif
}

inline double randDouble()
{
#ifdef _WIN32
    return 1.0 * randUInt() / (1 << 20);
#else
    return 1.0 * rand() / RAND_MAX;
#endif
}
}

#endif // CONST_H
