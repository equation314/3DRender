#ifndef POLYNOMIAL6_H
#define POLYNOMIAL6_H

#include <vector>

struct Polynomial6
{
public:
    Polynomial6(const long double a[7]);

    long double F(long double x);
    long double dF(long double x);
    int rootCount(long double l, long double r);
    std::vector<double> findAllRoots(double l, double r);

private:
    long double m_a[7], m_sturm[7][7];
    std::vector<double> m_roots;

    void m_init();
    bool m_findOneRoot(long double l, long double r, long double& x);
    void m_findAllRoots(long double l, long double r);
};

#endif // POLYNOMIAL6_H
