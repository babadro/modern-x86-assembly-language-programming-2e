//------------------------------------------------
//               Ch10_02.cpp
//------------------------------------------------

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include "YmmVal.h"

using namespace std;

struct alignas(32) YmmVal2
{
    YmmVal m_YmmVal0;
    YmmVal m_YmmVal1;
};

extern "C" YmmVal2 Avx2UnpackU32_U64_(const YmmVal& a, const YmmVal& b);
extern "C" void Avx2PackI32_I16_(const YmmVal& a, const YmmVal& b, YmmVal* c);

void Avx2UnpackU32_U64(void)
{
    alignas(32) YmmVal a;
    alignas(32) YmmVal b;

    a.m_U32[0] = 0x00000000;  b.m_U32[0] = 0x88888888;
    a.m_U32[1] = 0x11111111;  b.m_U32[1] = 0x99999999;
    a.m_U32[2] = 0x22222222;  b.m_U32[2] = 0xaaaaaaaa;
    a.m_U32[3] = 0x33333333;  b.m_U32[3] = 0xbbbbbbbb;

    a.m_U32[4] = 0x44444444;  b.m_U32[4] = 0xcccccccc;
    a.m_U32[5] = 0x55555555;  b.m_U32[5] = 0xdddddddd;
    a.m_U32[6] = 0x66666666;  b.m_U32[6] = 0xeeeeeeee;
    a.m_U32[7] = 0x77777777;  b.m_U32[7] = 0xffffffff;

    YmmVal2 c = Avx2UnpackU32_U64_(a, b);

    cout << "\nResults for Avx2UnpackU32_U64\n\n";

    cout << "a lo           " << a.ToStringX32(0) << '\n';
    cout << "b lo           " << b.ToStringX32(0) << '\n';
    cout << '\n';

    cout << "a hi           " << a.ToStringX32(1) << '\n';
    cout << "b hi           " << b.ToStringX32(1) << '\n';

    cout << "\nvpunpckldq result\n";
    cout << "c.m_YmmVal0 lo " << c.m_YmmVal0.ToStringX64(0) << '\n';
    cout << "c.m_YmmVal0 hi " << c.m_YmmVal0.ToStringX64(1) << '\n';

    cout << "\nvpunpckhdq result\n";
    cout << "c.m_YmmVal1 lo " << c.m_YmmVal1.ToStringX64(0) << '\n';
    cout << "c.m_YmmVal1 hi " << c.m_YmmVal1.ToStringX64(1) << '\n';
}

void Avx2PackI32_I16(void)
{
    alignas(32) YmmVal a;
    alignas(32) YmmVal b;
    alignas(32) YmmVal c;

    a.m_I32[0] = 10;          b.m_I32[0] = 32768;
    a.m_I32[1] = -200000;     b.m_I32[1] = 6500;
    a.m_I32[2] = 300000;      b.m_I32[2] = 42000;
    a.m_I32[3] = -4000;       b.m_I32[3] = -68000;

    a.m_I32[4] = 9000;        b.m_I32[4] = 25000;
    a.m_I32[5] = 80000;       b.m_I32[5] = 500000;
    a.m_I32[6] = 200;         b.m_I32[6] = -7000;
    a.m_I32[7] = -32769;      b.m_I32[7] = 12500;

    Avx2PackI32_I16_(a, b, &c);

    cout << "\nResults for Avx2PackI32_I16\n\n";

    cout << "a lo " << a.ToStringI32(0) << '\n';
    cout << "a hi " << a.ToStringI32(1) << '\n';
    cout << '\n';

    cout << "b lo " << b.ToStringI32(0) << '\n';
    cout << "b hi " << b.ToStringI32(1) << '\n';
    cout << '\n';

    cout << "c lo " << c.ToStringI16(0) << '\n';
    cout << "c hi " << c.ToStringI16(1) << '\n';
    cout << '\n';
}

int main()
{
    Avx2UnpackU32_U64();
    Avx2PackI32_I16();
    return 0;
}
