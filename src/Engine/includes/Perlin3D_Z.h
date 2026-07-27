#ifndef _PERLINARRAY3D_Z_H_
#define _PERLINARRAY3D_Z_H_
#include "Memory_Z.h"
#include "Math_Z.h"
#include <stdlib.h>

template <S32 SizeX, S32 SizeY, S32 SizeZ>
class PerlinArray3D_Z {
public:
    void Generate(Float i_Persistence, Float i_BaseFrequency, int i_OctaveCount) {
        m_Persistence = i_Persistence;
        m_OctaveCount = i_OctaveCount;
        GenerateNoise();
        for (S32 z = 0, i = 0; z < SizeZ; z++, i++) {
            for (S32 y = 0, j = 0; y < SizeY; y++, j++) {
                for (S32 x = 0; x < SizeX; x++) {
                    m_OutputNoise[x + (j * SizeY) + (i * SizeX * SizeY)] = PerlinNoise(x, y, z, i_BaseFrequency);
                }
            }
        }
    }

    // Fractal noise
    Float PerlinNoise(Float i_PosX, Float i_PosY, Float i_PosZ, Float i_BaseFrequency) {
        Float l_Lacunarity = 2.0f;
        Float l_Result = 0.0f;
        Float l_Persistence = m_Persistence;

        Float l_Frequency = i_BaseFrequency;

        for (S32 i = 0; i < m_OctaveCount; i++) {
            Float l_Amplitude = (std::powf(l_Persistence, i));
            Float l_Noise = (InterpolatedNoise(
                i_PosX * l_Frequency, i_PosY * l_Frequency, i_PosZ * l_Frequency
            ));
            l_Result = (l_Amplitude * l_Noise + l_Result);
            l_Frequency *= l_Lacunarity;
        }
        return l_Result;
    }

    // clang-format off

    // Lattice Noise
    Float InterpolatedNoise(Float i_PosX, Float i_PosY, Float i_PosZ) {
        Float l_FracX = i_PosX - std::floorf(i_PosX);
        Float l_FracY = i_PosY - std::floorf(i_PosY);
        Float l_FracZ = i_PosZ - std::floorf(i_PosZ);

        S32 l_CellX = (S32)(std::floorf(i_PosX) + SizeX) % SizeX;
        S32 l_CellY = (S32)(std::floorf(i_PosY) + SizeY) % SizeY;
        S32 l_CellZ = (S32)(std::floorf(i_PosZ) + SizeZ) % SizeZ;

        S32 l_CellX1 = (l_CellX + (SizeX - 1)) % SizeX;
        S32 l_CellY1 = (l_CellY + (SizeY - 1)) % SizeY;
        S32 l_CellZ1 = (l_CellZ + (SizeZ - 1)) % SizeZ;

        Float l_Acc = 0.0f;
        l_Acc += l_FracX          * l_FracY          * l_FracZ          * GetNoiseValue(l_CellX,   l_CellY,   l_CellZ);
        l_Acc += l_FracX          * (1.0f - l_FracY) * l_FracZ          * GetNoiseValue(l_CellX,   l_CellY1,  l_CellZ);
        l_Acc += (1.0f - l_FracX) * l_FracY          * l_FracZ          * GetNoiseValue(l_CellX1,  l_CellY,   l_CellZ);
        l_Acc += (1.0f - l_FracX) * (1.0f - l_FracY) * l_FracZ          * GetNoiseValue(l_CellX1,  l_CellY1,  l_CellZ);
        l_Acc += l_FracX          * l_FracY          * (1.0f - l_FracZ) * GetNoiseValue(l_CellX,   l_CellY,   l_CellZ1);
        l_Acc += l_FracX          * (1.0f - l_FracY) * (1.0f - l_FracZ) * GetNoiseValue(l_CellX,   l_CellY1,  l_CellZ1);
        l_Acc += (1.0f - l_FracX) * l_FracY          * (1.0f - l_FracZ) * GetNoiseValue(l_CellX1,  l_CellY,   l_CellZ1);
        l_Acc += (1.0f - l_FracX) * (1.0f - l_FracY) * (1.0f - l_FracZ) * GetNoiseValue(l_CellX1,  l_CellY1,  l_CellZ1);

        return l_Acc / 8.0f;
    }

    Float GetNoiseValue(Float i_X, Float i_Y, Float i_Z) {
        return m_GradientTable[((U32)i_X & (SizeX - 1)) + ((U32)i_Y & (SizeY - 1)) * SizeX + ((U32)i_Z & (SizeZ - 1)) * (SizeX * SizeY)];
    }

    // TODO: Finish matching
    void GenerateNoise() {
        Float* l_RandBuf = (Float*)AllocL_Z(SizeX * SizeY * SizeZ * sizeof(Float), 18);
        for (S32 i = 0; i < SizeX * SizeY * SizeZ; ++i) {
            Float l_Rand = rand();
            l_RandBuf[i] = 2.0f * (l_Rand / 32767.0f - 0.5f);
        }

        for (S32 z = 0; z < SizeZ; z++) {
            S32 l_ZBase = z * (SizeX * SizeY);

            for (S32 y = 0; y < SizeY; y++) {
                S32 l_RowMinus = l_ZBase + ((y - 1) & (SizeY - 1)) * SizeX;
                S32 l_RowCenter = l_ZBase + y * SizeX;
                S32 l_RowPlus = l_ZBase + ((y + 1) & (SizeY - 1)) * SizeX;

                for (S32 x = 0; x < SizeX; x++) {
                    S32 l_XMinus1 = (x - 1) & (SizeX - 1);
                    S32 l_XPlus1 = (x + 1) & (SizeX - 1);

                    Float l_Sum = l_RandBuf[l_RowMinus + l_XMinus1] + l_RandBuf[l_RowMinus + x] + l_RandBuf[l_RowMinus + l_XPlus1] +
                                  l_RandBuf[l_RowCenter + l_XMinus1] + 6.0f * l_RandBuf[l_RowCenter + x] + l_RandBuf[l_RowCenter + l_XPlus1] +
                                  l_RandBuf[l_RowPlus + l_XMinus1] + l_RandBuf[l_RowPlus + x] + l_RandBuf[l_RowPlus + l_XPlus1];

                    m_GradientTable[l_RowCenter + x] = (1.0f / 14.0f) * l_Sum;
                }
            }
        }

        Free_Z(l_RandBuf);
    }

    // clang-format on

private:
    Float m_Persistence;
    S32 m_OctaveCount;
    Float m_OutputNoise[SizeX * SizeY * SizeZ];
    Float m_GradientTable[SizeX * SizeY * SizeZ];
};

#endif
