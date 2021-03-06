//------------------------------------------------
//               Ch16_02.cpp
//------------------------------------------------

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include "Ch16_02.h"
#include "AlignedMem.h"

using namespace std;

void Init(float* x, size_t n, unsigned int seed)
{
    uniform_int_distribution<> ui_dist {1, 1000};
    default_random_engine rng {seed};

    for (size_t i = 0; i < n; i++)
        x[i] = (float)ui_dist(rng);
}

bool CalcResultCpp(float* c, const float* a, const float* b, size_t n)
{
    size_t align = 32;

    if ((n == 0) || ((n & 0x0f) != 0))
        return false;

    if (!AlignedMem::IsAligned(a, align))
        return false;
    if (!AlignedMem::IsAligned(b, align))
        return false;
    if (!AlignedMem::IsAligned(b, align))
        return false;

    for (size_t i = 0; i < n; i++)
        c[i] = sqrt(a[i] * a[i] + b[i] * b[i]);

    return true;
}

void CompareResults(const float* c1, const float* c2a, const float*c2b, size_t n)
{
    bool compare_ok = true;
    const float epsilon = 1.0e-9f;

    cout << fixed << setprecision(4);

    for (size_t i = 0; i < n && compare_ok; i++)
    {
        bool b1 = fabs(c1[i] - c2a[i]) > epsilon;
        bool b2 = fabs(c1[i] - c2b[i]) > epsilon;

        cout << setw(2) << i << " - ";
        cout << setw(10) << c1[i] << ' ';
        cout << setw(10) << c2a[i] << ' ';
        cout << setw(10) << c2b[i] << '\n';

        if (b1 || b2)
            compare_ok = false;
    }

    if (compare_ok)
        cout << "Array compare OK\n";
    else
        cout << "Array compare FAILED\n";
}

void NonTemporalStore(void)
{
    const size_t n = 16;
    const size_t align = 32;

    AlignedArray<float> a_aa(n, align);
    AlignedArray<float> b_aa(n, align);
    AlignedArray<float> c1_aa(n, align);
    AlignedArray<float> c2a_aa(n, align);
    AlignedArray<float> c2b_aa(n, align);
    float* a = a_aa.Data();
    float* b = b_aa.Data();
    float* c1 = c1_aa.Data();
    float* c2a = c2a_aa.Data();
    float* c2b = c2b_aa.Data();

    Init(a, n, 67);
    Init(b, n, 79);

    bool rc1 = CalcResultCpp(c1, a, b, n);
    bool rc2 = CalcResultA_(c2a, a, b, n);
    bool rc3 = CalcResultB_(c2b, a, b, n);

    if (!rc1 || !rc2 || !rc3)
    {
        cout << "Invalid return code\n";
        cout << "rc1 = " << boolalpha << rc1 << '\n';
        cout << "rc2 = " << boolalpha << rc2 << '\n';
        cout << "rc3 = " << boolalpha << rc3 << '\n';
        return;
    }

    cout << "Results for NonTemporalStore\n";
    CompareResults(c1, c2a, c2b, n);
}

int main()
{
    NonTemporalStore();
    NonTemporalStore_BM();
    return 0;
}
