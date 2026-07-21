#ifndef _GCSOUND_Z_H_
#define _GCSOUND_Z_H_
#include "Sound_Z.h"
#include <ax.h>

class DSPADPCM {
public:
    U32 m_NumSamples;
    U32 m_NumAdpcmNibbles;
    U32 m_SampleRate;
    U16 m_LoopFlag;
    U16 m_Format;
    U32 m_LoopAddress;
    U32 m_EndAddress;
    U32 m_CurrentAddress;
    U16 m_Coefficients[16];
    U16 m_Gain;
    U16 m_PredictorScale;
    U16 m_Yn1;
    U16 m_Yn2;
    U16 m_LoopPredictorScale;
    U16 m_LoopYn1;
    U16 m_LoopYn2;
    U16 m_Padding[11];
};

class SndDatas_Z {
public:
    AXPBADDR m_Address;
    AXPBSRC m_Source;
    AXPBADPCM m_Adpcm;
    AXPBADPCMLOOP m_Loop;
};
#endif // _GCSOUND_Z_H_
