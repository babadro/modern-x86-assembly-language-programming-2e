//------------------------------------------------
//               Ch09_07.cpp
//------------------------------------------------

#include "stdafx.h"
#include <cstdint>
#include <iostream>
#include "YmmVal.h"

using namespace std;

extern "C" void AvxBlendF32_(YmmVal* des1, YmmVal* src1, YmmVal* src2, YmmVal* idx1);
extern "C" void Avx2PermuteF32_(YmmVal* des1, YmmVal* src1, YmmVal* idx1, YmmVal* des2, YmmVal* src2, YmmVal* idx2);

void AvxBlendF32(void)
{
    const uint32_t sel0 = 0x00000000;
    const uint32_t sel1 = 0x80000000;
    alignas(32) YmmVal des1, src1, src2, idx1;

    src1.m_F32[0] = 10.0f;  src2.m_F32[0] = 100.0f;  idx1.m_I32[0] = sel1;
    src1.m_F32[1] = 20.0f;  src2.m_F32[1] = 200.0f;  idx1.m_I32[1] = sel0;
    src1.m_F32[2] = 30.0f;  src2.m_F32[2] = 300.0f;  idx1.m_I32[2] = sel0;
    src1.m_F32[3] = 40.0f;  src2.m_F32[3] = 400.0f;  idx1.m_I32[3] = sel1;
    src1.m_F32[4] = 50.0f;  src2.m_F32[4] = 500.0f;  idx1.m_I32[4] = sel1;
    src1.m_F32[5] = 60.0f;  src2.m_F32[5] = 600.0f;  idx1.m_I32[5] = sel0;
    src1.m_F32[6] = 70.0f;  src2.m_F32[6] = 700.0f;  idx1.m_I32[6] = sel1;
    src1.m_F32[7] = 80.0f;  src2.m_F32[7] = 800.0f;  idx1.m_I32[7] = sel0;

    AvxBlendF32_(&des1, &src1, &src2, &idx1);

    cout << "\nResults for AvxBlendF32 (vblendvps)\n";
    cout << fixed << setprecision(1);

    for (size_t i = 0; i < 8; i++)
    {
        cout << "i: " << setw(2) << i << "  ";
        cout << "src1: " << setw(8) << src1.m_F32[i] << "  ";
        cout << "src2: " << setw(8) << src2.m_F32[i] << "  ";
        cout << setfill('0');
        cout << "idx1: 0x" << setw(8) << hex << idx1.m_U32[i] << "  ";
        cout << setfill(' ');
        cout << "des1: " << setw(8) << des1.m_F32[i] << '\n';
    }
}

void Avx2PermuteF32(void)
{
    alignas(32) YmmVal des1, src1, idx1;
    alignas(32) YmmVal des2, src2, idx2;

    // idx1 values must be between 0 and 7.
    src1.m_F32[0] = 100.0f;       idx1.m_I32[0] = 3;
    src1.m_F32[1] = 200.0f;       idx1.m_I32[1] = 7;
    src1.m_F32[2] = 300.0f;       idx1.m_I32[2] = 0;
    src1.m_F32[3] = 400.0f;       idx1.m_I32[3] = 4;
    src1.m_F32[4] = 500.0f;       idx1.m_I32[4] = 6;
    src1.m_F32[5] = 600.0f;       idx1.m_I32[5] = 6;
    src1.m_F32[6] = 700.0f;       idx1.m_I32[6] = 1;
    src1.m_F32[7] = 800.0f;       idx1.m_I32[7] = 2;

    // idx2 values must be between 0 and 3.
    src2.m_F32[0] = 100.0f;       idx2.m_I32[0] = 3;
    src2.m_F32[1] = 200.0f;       idx2.m_I32[1] = 1;
    src2.m_F32[2] = 300.0f;       idx2.m_I32[2] = 1;
    src2.m_F32[3] = 400.0f;       idx2.m_I32[3] = 2;
    src2.m_F32[4] = 500.0f;       idx2.m_I32[4] = 3;
    src2.m_F32[5] = 600.0f;       idx2.m_I32[5] = 2;
    src2.m_F32[6] = 700.0f;       idx2.m_I32[6] = 0;
    src2.m_F32[7] = 800.0f;       idx2.m_I32[7] = 0;

    Avx2PermuteF32_(&des1, &src1, &idx1, &des2, &src2, &idx2);

    cout << "\nResults for Avx2PermuteF32 (vpermps)\n";
    cout << fixed << setprecision(1);

    for (size_t i = 0; i < 8; i++)
    {
        cout << "i: " << setw(2) << i << "  ";
        cout << "src1: " << setw(8) << src1.m_F32[i] << "  ";
        cout << "idx1: " << setw(8) << idx1.m_I32[i] << "  ";
        cout << "des1: " << setw(8) << des1.m_F32[i] << '\n';
    }

    cout << "\nResults for Avx2PermuteF32 (vpermilps)\n";

    for (size_t i = 0; i < 8; i++)
    {
        cout << "i: " << setw(2) << i << "  ";
        cout << "src2: " << setw(8) << src2.m_F32[i] << "  ";
        cout << "idx2: " << setw(8) << idx2.m_I32[i] << "  ";
        cout << "des2: " << setw(8) << des2.m_F32[i] << '\n';
    }
}

int main()
{
    AvxBlendF32();
    Avx2PermuteF32();
    return 0;
}
