#include "GCSndStream_Z.h"
#include "GCMain_Z.h"
#include "GCSound_Z.h"
#include "Math_Z.h"
#include "Memory_Z.h"
#include "StreamManager_Z.h"
#include "XRamManager_Z.h"
#include <dolphin/os/OSCache.h>
#include <mix.h>

void GCSndStream_Z::Opened(S32 i_Error, S32 i_Size, S32 i_Offset) {
    if (!i_Error) {
        StreamSwap();
        StreamSwap();
    }
}

void GCSndStream_Z::Readed(S32 i_Error, S32 i_Size, S32 i_Offset) {
    if (!i_Error && i_Offset != -1) {
        Bool l_HasSize = i_Size != 0;
        m_BufferedSize += i_Size;
        m_PendingReadCount -= l_HasSize;
        U32 l_RemainingSize = 32768 - i_Size;
        if (l_RemainingSize) {
            memset(m_StreamBuffer.GetArrayPtr() + i_Offset + i_Size, 0, l_RemainingSize);
        }
        DCStoreRange(m_StreamBuffer.GetArrayPtr() + i_Offset, 32768);

        AXPBADPCMLOOP l_AdpcmLoop;
        U32 l_BufferOffset;
        U32 l_EndOffset = i_Offset + i_Size;
        S32 l_VoiceIndex = 0;
        l_AdpcmLoop.loop_yn2 = l_AdpcmLoop.loop_yn1 = l_BufferOffset = 0;
        do {
            if ((U32)i_Offset <= l_BufferOffset && l_BufferOffset < l_EndOffset) {
                l_AdpcmLoop.loop_pred_scale = m_StreamBuffer[l_BufferOffset];
                if (m_ChannelCount == 2.0f) {
                    AXSetVoiceAdpcmLoop(m_Voices[l_VoiceIndex & 1], &l_AdpcmLoop);
                }
                else if (!l_VoiceIndex) {
                    AXSetVoiceAdpcmLoop(m_Voices[0], &l_AdpcmLoop);
                }
            }
            l_VoiceIndex++;
            l_BufferOffset += 16384;
        } while (l_VoiceIndex < 4);

        U32 l_BaseAddress = gData.XRamMgr->GetBlockPtr((S16)m_Unk_0x48);
        gData.XRamMgr->Write(
            m_StreamBuffer.GetArrayPtr() + i_Offset,
            l_BaseAddress + i_Offset,
            l_RemainingSize + i_Size,
            FALSE
        );
        gData.XRamMgr->Flush();
    }
}

Bool GCSndStream_Z::Read(S32 i_Size, S32 i_BufferOffset) {
    if (i_Size && gData.StreamMgr->Read(m_StreamBuffer.GetArrayPtr() + i_BufferOffset, m_StreamReadOffset, i_Size, this, i_BufferOffset)) {
        m_PendingReadCount++;
    }
    m_StreamWritePosition += i_Size;
    m_StreamReadOffset += i_Size;
    m_StreamSize -= i_Size;
    S32 l_BufferedSize = m_StreamEndPosition - m_StreamWritePosition;
    if (!i_Size) {
        Readed(0, 0, i_BufferOffset);
    }
    m_BufferedSize += l_BufferedSize;
    return TRUE;
}

Bool GCSndStream_Z::SetVpb(AXVPB* i_Voice, SndDatas_Z* i_LeftData, SndDatas_Z* i_RightData) {
    S16 l_XRamBlock = gData.XRamMgr->AllocBlock(65536, -1, -1);
    m_Unk_0x48 = l_XRamBlock;
    if (m_Unk_0x48 < 0) {
        return FALSE;
    }

    m_Unk_0x3C = gData.m_FrameCount;
    m_Muted = FALSE;
    gData.XRamMgr->GetBlockPtr(l_XRamBlock);
    m_Voices[0] = i_Voice;
    AXSetVoiceType(i_Voice, GCSND_VOICE_TYPE_STREAM);

    AXPBADPCM l_Adpcm;
    memcpy(l_Adpcm.a, i_LeftData->m_Adpcm.a, sizeof(l_Adpcm.a));
    l_Adpcm.gain = 0;
    l_Adpcm.pred_scale = i_LeftData->m_Adpcm.pred_scale;
    l_Adpcm.yn1 = 0;
    l_Adpcm.yn2 = 0;
    AXSetVoiceAdpcm(i_Voice, &l_Adpcm);

    AXPBSRC l_Source;
    l_Source.ratioHi = 1;
    l_Source.ratioLo = 0;
    l_Source.currentAddressFrac = 0;
    l_Source.last_samples[0] = 0;
    l_Source.last_samples[1] = 0;
    l_Source.last_samples[2] = 0;
    l_Source.last_samples[3] = 0;
    AXSetVoiceSrcType(i_Voice, AX_SRC_TYPE_LINEAR);
    AXSetVoiceSrc(i_Voice, &l_Source);

    if (!i_RightData) {
        SetAddr(0, 65536, 0, 0);
        AXSetVoiceState(i_Voice, GCSND_VOICE_STATE_STOP);
        MIXInitChannel(i_Voice, GCSND_MIX_MODE_UNKNOWN_4, 0, -904, -904, 64, 127, 0);
        return TRUE;
    }

    SetAddr(0, 16384, 32768, 0);
    AXSetVoiceState(i_Voice, GCSND_VOICE_STATE_STOP);
    MIXInitChannel(i_Voice, GCSND_MIX_MODE_UNKNOWN_4, 0, -904, -904, 0, 127, 0);
    AXVPB* l_RightVoice = AXAcquireVoice(GCSND_STREAM_VOICE_PRIORITY, NULL, NULL);
    m_Voices[1] = l_RightVoice;
    if (!l_RightVoice) {
        gData.XRamMgr->FreeBlock(l_XRamBlock);
        m_Unk_0x48 = -1;
        return FALSE;
    }

    AXSetVoiceType(l_RightVoice, GCSND_VOICE_TYPE_STREAM);
    memcpy(l_Adpcm.a, i_RightData->m_Adpcm.a, sizeof(l_Adpcm.a));
    l_Adpcm.pred_scale = i_RightData->m_Adpcm.pred_scale;
    AXSetVoiceAdpcm(l_RightVoice, &l_Adpcm);
    AXSetVoiceSrcType(l_RightVoice, AX_SRC_TYPE_LINEAR);
    AXSetVoiceSrc(l_RightVoice, &l_Source);
    SetAddr(1, 32768, 49152, 16384);
    AXSetVoiceState(l_RightVoice, GCSND_VOICE_STATE_STOP);
    MIXInitChannel(l_RightVoice, GCSND_MIX_MODE_UNKNOWN_4, 0, -904, -904, 127, 127, 0);
    return TRUE;
}

void GCSndStream_Z::SetAddr(
    S32 i_VoiceIndex,
    S32 i_EndAddress,
    S32 i_LoopAddress,
    S32 i_CurrentAddress
) {
    AXPBADDR l_Address;
    U32 l_BaseAddress = gData.XRamMgr->GetBlockPtr((S16)m_Unk_0x48);
    i_EndAddress *= 2;
    l_BaseAddress *= 2;
    i_LoopAddress *= 2;
    i_EndAddress += l_BaseAddress;
    i_LoopAddress += l_BaseAddress;
    if (i_CurrentAddress != -1) {
        S32 l_EndAddress;
        S32 l_LoopAddress;
        l_LoopAddress = i_LoopAddress + 2;
        l_EndAddress = i_EndAddress - 1;
        i_CurrentAddress *= 2;
        i_CurrentAddress += l_BaseAddress;
        U32 l_CurrentAddress = i_CurrentAddress + 2;
        U16 l_LoopAddressHi = (U32)l_LoopAddress >> 16;
        U16 l_EndAddressHi = (U32)l_EndAddress >> 16;
        U16 l_CurrentAddressHi = (U32)l_CurrentAddress >> 16;
        l_Address.loopFlag = 1;
        l_Address.loopAddressHi = l_LoopAddressHi;
        l_Address.loopAddressLo = l_LoopAddress;
        l_Address.endAddressHi = l_EndAddressHi;
        l_Address.format = 0;
        l_Address.endAddressLo = l_EndAddress;
        l_Address.currentAddressHi = l_CurrentAddressHi;
        l_Address.currentAddressLo = l_CurrentAddress;
        AXSetVoiceAddr(m_Voices[i_VoiceIndex], &l_Address);
    }
    else {
        AXSetVoiceEndAddr(m_Voices[i_VoiceIndex], i_EndAddress - 1);
        AXSetVoiceLoopAddr(m_Voices[i_VoiceIndex], i_LoopAddress + 2);
    }
}

Bool GCSndStream_Z::Process(S32 i_Param) {
    if (m_StreamBufferNeedsCheck && !m_Muted) {
        Check();
    }
    Bool l_Result;
    if (m_PendingReadCount || m_Muted) {
        l_Result = TRUE;
    }
    else {
        if (m_BufferedSize >= 65536 || m_StreamBufferNeedsCheck) {
            l_Result = FALSE;
            m_StreamBufferNeedsCheck = TRUE;
        }
        else {
            l_Result = TRUE;
        }
    }
    return l_Result;
}

S32 GCSndStream_Z::StreamSwap() {
    S32 l_Result;
    if (m_StreamSize == -1) {
        l_Result = FALSE;
    }
    else if (m_StreamSize == 0) {
        m_StreamSize--;
        l_Result = Stream(32768, m_StreamBufferIndex << 15);
    }
    else if ((m_StreamFlags & GCSND_STREAM_FLAG_UNKNOWN_8) || m_MusicSkipInfos.GetSize()) {
        l_Result = Stream(32768, m_StreamBufferIndex << 15);
    }
    else {
        l_Result = Stream(Min<S32>(m_StreamSize, 32768), m_StreamBufferIndex << 15);
    }
    return l_Result;
}

void GCSndStream_Z::SetVolume(Float i_Volume, Float i_Input) {
    if (m_Voices[1]) {
        Bool l_IsMuted = MIXIsMute(m_Voices[1]);
        if (i_Volume <= 0.0f) {
            if (!l_IsMuted) {
                MIXMute(m_Voices[1]);
            }
        }
        else if (l_IsMuted) {
            MIXUnMute(m_Voices[1]);
        }
        MIXSetInput(m_Voices[1], (S32)i_Input);
    }
}

void GCSndStream_Z::SetFrequency(Float i_Frequency) {
    if (m_Voices[1]) {
        AXSetVoiceSrcRatio(m_Voices[1], i_Frequency / 32000.0f);
    }
}

Float GCSndStream_Z::SetPan(Float i_Pan, Float i_SurroundPan) {
    return i_Pan;
}

void GCSndStream_Z::Run() {
    if (m_Voices[1]) {
        AXSetVoiceState(m_Voices[1], GCSND_VOICE_STATE_RUN);
    }
}
