#include "Gaussian_Z.h"
#include <math.h>

// TODO: Finish matching
S32 Compute_MipmapedGaussConvol(MipmapGC* i_Mip, int i_SizeX, int i_SizeY, Float i_UnkFloat, Float i_UnkFloat2) {
    Float l_Gauss[128];
    Float l_Values[16];
    S32 i;

    Float l_Sigma = 0.25f * (1.0f + i_UnkFloat);
    S32 l_NbTap = l_Sigma <= 0.0f ? 0 : (S32)(Float)ceil(4.0f * l_Sigma);

    if (l_NbTap < 2 || l_NbTap > 128) {
        return 0;
    }

    S32 l_Nb = l_Sigma <= 0.0f ? 0 : (S32)(Float)ceil(4.0f * l_Sigma);

    if (l_Nb > 0) {
        Float l_Sum = 0.0f;
        Float l_Div = 2.0f * l_Sigma * l_Sigma;

        for (i = 0; i < l_Nb; i++) {
            Float l_X = (Float)i;
            Float l_Value = (Float)exp(-(l_X * l_X) / l_Div);

            l_Sum += l_Value;
            l_Gauss[i] = l_Value;
        }

        Float l_Inv = 1.0f / l_Sum;

        for (i = 0; i < l_Nb; i++) {
            l_Gauss[i] *= l_Inv;
        }
    }

    S32 l_Size = Min(i_SizeX, i_SizeY);
    S32 l_NbMip = 0;

    while ((l_Size >> (l_NbMip + 1)) >= 8) {
        l_NbMip++;
    }

    if (!l_NbMip || l_NbMip >= 16) {
        return 0;
    }

    Float l_Prev = 0.0f;
    MipmapGCEle* l_Ele = &i_Mip->m_Eles[l_NbMip];
    Float* l_Value = &l_Values[l_NbMip];

    l_Values[l_NbMip + 1] = 0.0f;

    for (i = l_NbMip; i >= 0; i--) {
        S32 l_Cur = 1 << i;
        S32 l_Max = Min(l_Cur, l_NbTap);
        S32 l_Min = Min(l_Cur >> 1, l_NbTap);
        Float l_Acc = 0.0f;

        for (S32 j = l_Min; j < l_Max; j++) {
            l_Acc += l_Gauss[j];
        }

        Float l_Avg = l_Max > l_Min ? l_Acc / (Float)(l_Max - l_Min) : 0.0f;
        Float l_Delta = l_Avg - l_Prev;

        l_Prev = l_Avg;

        Float l_Scale = 1.0f / (Float)l_Cur;
        Float l_Weight = l_Delta / l_Scale;

        l_Ele->m_Weight = l_Weight;
        *l_Value = l_Weight + l_Value[1];
        l_Ele->m_Enabled = i_UnkFloat2 >= l_Scale;

        l_Value--;
        l_Ele--;
    }

    i_Mip->m_Eles[0].m_Level = (S32)(128.0f * l_Values[0]);

    S32 l_Last = l_NbMip;

    for (i = l_NbMip; i > 0; i--) {
        Float l_Cur = *l_Value;
        Float l_Prv = l_Value[-1];
        Bool l_Valid = l_Cur != 0.0f && l_Prv != 0.0f;
        S32 l_Level = (S32)(128.0f * (l_Valid ? l_Cur / l_Prv : 0.0f));

        if (!l_Level) {
            l_Last = i - 1;
        }
        else {
            l_Ele->m_Level = l_Level;
        }

        l_Value--;
        l_Ele--;
    }

    return l_Last;
}
