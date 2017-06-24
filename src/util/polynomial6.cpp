#include "common/config.h"
#include "common/const.h"
#include "util/polynomial6.h"

#include <cstring>
#include <iostream>

const long double EPS = 1e-10;
const long double SMALL_EPS = 1e-14;
const long double TINY_EPS = 1e-20;

Polynomial6::Polynomial6(const long double a[7])
{
    for (int i = 0; i < 7; i++) m_a[i] = a[i];
    m_init();
}

long double Polynomial6::F(long double x)
{
    long double f = 0;
    for (int i = 6; i >= 0; i--) f = f * x + m_a[i];
    return f;
}

long double Polynomial6::dF(long double x)
{
    long double f = 0;
    for (int i = 5; i >= 0; i--) f = f * x + m_sturm[1][i];
    return f;
}

std::vector<double> Polynomial6::findAllRoots(double l, double r)
{
    m_roots.clear();
    m_findAllRoots(l, r);
    if (std::abs(F(l)) < SMALL_EPS) m_roots.push_back(l);
    if (std::abs(F(r)) < SMALL_EPS) m_roots.push_back(r);
    return m_roots;
}

int Polynomial6::rootCount(long double l, long double r)
{
    int s = 0;
    long double fl, fr, gl = 0, gr = 0;
    if (r - l < EPS) return 0;
    for (int i = 0; i < 7; i++)
    {
        if (m_sturm[i][6 - i] == 0) break;
        fl = fr = 0;
        for (int j = 6 - i; j >= 0; j--)
        {
            fl = fl * l + m_sturm[i][j];
            fr = fr * r + m_sturm[i][j];
        }
        if (std::abs(fl) > TINY_EPS)
        {
            if (fl * gl < 0) s++;
            gl = fl;
        }
        if (std::abs(fr) > TINY_EPS)
        {
            if (fr * gr < 0) s--;
            gr = fr;
        }
    }
    return s;
}

void Polynomial6::m_init()
{
    memset(m_sturm, 0, sizeof(m_sturm));
    for (int i = 0; i < 7; i++)
    {
        m_sturm[0][i] = m_a[i];
        if (i < 6) m_sturm[1][i] = (i + 1) * m_a[i + 1];
    }
    for (int i = 2; i < 7; i++)
    {
        long double a = m_sturm[i - 2][8 - i] / m_sturm[i - 1][7 - i],
                    b = (m_sturm[i - 2][7 - i] - a * m_sturm[i - 1][6 - i]) / m_sturm[i - 1][7 - i];
        for (int j = 0; j < 7 - i; j++)
            m_sturm[i][j] = (j ? a * m_sturm[i - 1][j - 1] : 0) + b * m_sturm[i - 1][j] - m_sturm[i - 2][j];
        if (std::abs(m_sturm[i][6 - i]) < TINY_EPS)
        {
            for (int j = 0; j < 7 - i; j++) m_sturm[i][j] = 0;
            break;
        }
    }
}

bool Polynomial6::m_findOneRoot(long double l, long double r, long double& x)
{
    x = l;
    for (int i = 0; i < Config::newton_iteration_depth; i++)
    {
        long double f = F(x), d = dF(x), dx;
        if (std::abs(f) < SMALL_EPS) return l < x - EPS && x < r - EPS;
        if (std::abs(d) < EPS) break;
        dx = f / d, x -= dx;
        if (std::abs(dx) < EPS) break;
    }
    return std::abs(x) < SMALL_EPS && l < x - EPS && x < r - EPS;
}

void Polynomial6::m_findAllRoots(long double l, long double r)
{
    if (r - l < EPS)
    {
        if (std::abs(F(l)) < SMALL_EPS) m_roots.push_back(l);
        return;
    }
    int c = rootCount(l, r);
    if (c == 0)
        return;
    else if (c == 1)
    {
        long double x;
        if (m_findOneRoot(l, r, x))
        {
            m_roots.push_back(x);
            return;
        }
    }
    long double m = (l + r) / 2;
    m_findAllRoots(l, m);
    m_findAllRoots(m, r);
}
