#ifndef CONST_H
#define CONST_H

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace Const
{
const double EPS = 1e-6;
const double PI = acos(-1.0);

inline double randDouble()
{
    return 1.0 * rand() / RAND_MAX;
}
}

#endif // CONST_H
