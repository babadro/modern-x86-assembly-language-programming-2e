//------------------------------------------------
//               Ch09_04.cpp
//------------------------------------------------

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include "AlignedMem.h"

using namespace std;

extern "C" bool AvxCalcCorrCoef_(const double* x, const double* y, size_t n, double sums[5], double epsilon, double* rho);

void Init(double* x, double* y, size_t n, unsigned int seed)
{
    uniform_int_distribution<> ui_dist {1, 999};
    default_random_engine rng {seed};

    for (size_t i = 0; i < n; i++)
    {
        x[i] = (double)ui_dist(rng);
        y[i] = x[i] + (ui_dist(rng) % 6000) - 3000;
    }
}

bool AvxCalcCorrCoefCpp(const double* x, const double* y, size_t n, double sums[5], double epsilon, double* rho)
{
    const size_t alignment = 32;

    // Make sure n is valid
    if (n == 0)
        return false;

    // Make sure x and y are properly aligned
    if (!AlignedMem::IsAligned(x, alignment))
        return false;
    if (!AlignedMem::IsAligned(y, alignment))
        return false;

    // Calculate and save sum variables
    double sum_x = 0, sum_y = 0, sum_xx = 0, sum_yy = 0, sum_xy = 0;

    for (size_t i = 0; i < n; i++)
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_xx += x[i] * x[i];
        sum_yy += y[i] * y[i];
        sum_xy += x[i] * y[i];
    }

    sums[0] = sum_x;
    sums[1] = sum_y;
    sums[2] = sum_xx;
    sums[3] = sum_yy;
    sums[4] = sum_xy;

    // Calculate rho
    double rho_num = n * sum_xy - sum_x * sum_y;
    double rho_den = sqrt(n * sum_xx - sum_x * sum_x) * sqrt(n * sum_yy - sum_y * sum_y);

    if (rho_den >= epsilon)
    {
        *rho = rho_num / rho_den;
        return true;
    }
    else
    {
        *rho = 0;
        return false;
    }
}

int main()
{
    const size_t n = 103;
    const size_t alignment = 32;
    AlignedArray<double> x_aa(n, alignment);
    AlignedArray<double> y_aa(n, alignment);
    double sums1[5], sums2[5];
    double rho1, rho2;
    double epsilon = 1.0e-12;
    double* x = x_aa.Data();
    double* y = y_aa.Data();

    Init(x, y, n, 71);

    bool rc1 = AvxCalcCorrCoefCpp(x, y, n, sums1, epsilon, &rho1);
    bool rc2 = AvxCalcCorrCoef_(x, y, n, sums2, epsilon, &rho2);

    cout << "Results for AvxCalcCorrCoef\n\n";

    if (!rc1 || !rc2)
    {
        cout << "Invalid return code ";
        cout << "rc1 = " << boolalpha << rc1 << ", ";
        cout << "rc2 = " << boolalpha << rc2 << '\n';
        return 1;
    }

    int w = 14;
    string sep(w * 3, '-');

    cout << fixed << setprecision(8);
    cout << "Value    " << setw(w) << "C++" << " " << setw(w) << "x86-AVX" << '\n';
    cout << sep << '\n';
    cout << "rho:     " << setw(w) << rho1 << " " << setw(w) << rho2 << "\n\n";

    cout << setprecision(1);
    cout << "sum_x:   " << setw(w) << sums1[0] << " " << setw(w) << sums2[0] << '\n';
    cout << "sum_y:   " << setw(w) << sums1[1] << " " << setw(w) << sums2[1] << '\n';
    cout << "sum_xx:  " << setw(w) << sums1[2] << " " << setw(w) << sums2[2] << '\n';
    cout << "sum_yy:  " << setw(w) << sums1[3] << " " << setw(w) << sums2[3] << '\n';
    cout << "sum_xy:  " << setw(w) << sums1[4] << " " << setw(w) << sums2[4] << '\n';
    return 0;
}
