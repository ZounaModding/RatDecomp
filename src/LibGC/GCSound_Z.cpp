#include "GCSound_Z.h"
#include "GCMain_Z.h"
#include "Memory_Z.h"
#include "Sys_Z.h"
#include "XRamManager_Z.h"

void Sound_Z::Load(void** i_Data) {
    LOADRANGE_Z(m_Frequency, m_Unk0x1D);

    if (m_SoundSize) {
        DSPADPCM l_Header;
        LOAD_Z(l_Header);
        m_Frequency = l_Header.m_SampleRate;

        S32 l_SoundDataSize = m_SoundSize - sizeof(DSPADPCM);
        U8* l_SoundData = (U8*)*i_Data;
        *i_Data = l_SoundData + l_SoundDataSize;

        S32 l_XRamBlock = gData.XRamMgr->AllocBlock(l_SoundDataSize, -1, -1);
        m_XRamBlock = l_XRamBlock;
        if (l_XRamBlock >= 0) {
            U32 l_XRamAddress = gData.XRamMgr->GetBlockPtr(l_XRamBlock);
            gData.XRamMgr->Write(l_SoundData, l_XRamAddress, l_SoundDataSize, FALSE);

            SndDatas_Z* l_SoundDatas = (SndDatas_Z*)AllocCL_Z(sizeof(SndDatas_Z), "SndDatas_Z", 44);
            m_Data = l_SoundDatas;
            U32 l_BaseAddress = l_XRamAddress * 2;
            m_SoundSize -= sizeof(DSPADPCM);

            l_SoundDatas->m_Address.loopFlag = l_Header.m_LoopFlag;
            l_SoundDatas->m_Address.format = 0;

            l_SoundDatas->m_Address.loopAddressHi = (l_Header.m_LoopAddress + l_BaseAddress) >> 16;
            l_SoundDatas->m_Address.loopAddressLo = l_Header.m_LoopAddress + l_BaseAddress;
            l_SoundDatas->m_Address.endAddressHi = (l_Header.m_EndAddress + l_BaseAddress) >> 16;
            l_SoundDatas->m_Address.endAddressLo = l_Header.m_EndAddress + l_BaseAddress;
            l_SoundDatas->m_Address.currentAddressHi = (l_Header.m_CurrentAddress + l_BaseAddress) >> 16;
            l_SoundDatas->m_Address.currentAddressLo = l_Header.m_CurrentAddress + l_BaseAddress;

            memcpy(l_SoundDatas->m_Adpcm.a, l_Header.m_Coefficients, sizeof(l_Header.m_Coefficients));
            l_SoundDatas->m_Adpcm.gain = l_Header.m_Gain;
            l_SoundDatas->m_Adpcm.pred_scale = l_Header.m_PredictorScale;
            l_SoundDatas->m_Adpcm.yn1 = l_Header.m_Yn1;
            l_SoundDatas->m_Adpcm.yn2 = l_Header.m_Yn2;

            l_SoundDatas->m_Loop.loop_pred_scale = l_Header.m_LoopPredictorScale;
            l_SoundDatas->m_Loop.loop_yn1 = l_Header.m_LoopYn1;
            l_SoundDatas->m_Loop.loop_yn2 = l_Header.m_LoopYn2;

            l_SoundDatas->m_Source.ratioHi = 1;
            l_SoundDatas->m_Source.ratioLo = 0;
            l_SoundDatas->m_Source.currentAddressFrac = 0;
            l_SoundDatas->m_Source.last_samples[0] = 0;
            l_SoundDatas->m_Source.last_samples[1] = 0;
            l_SoundDatas->m_Source.last_samples[2] = 0;
            l_SoundDatas->m_Source.last_samples[3] = 0;
        }
    }
}
