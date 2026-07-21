#ifndef _GCSNDSTREAM_Z_H_
#define _GCSNDSTREAM_Z_H_

#include "BaseStream_Z.h"
#include "DynArray_Z.h"
#include "Name_Z.h"
#include "String_Z.h"

#include <ax.h>

class SndDatas_Z;

enum GCSndVoiceState_Z {
    GCSND_VOICE_STATE_STOP = 0,
    GCSND_VOICE_STATE_RUN = 1
};

enum GCSndVoiceType_Z {
    GCSND_VOICE_TYPE_STREAM = 1
};

enum GCSndMixMode_Z {
    GCSND_MIX_MODE_UNKNOWN_4 = 4
};

#define GCSND_STREAM_VOICE_PRIORITY 31

enum GCSndStreamFlags_Z {
    GCSND_STREAM_FLAG_UNKNOWN_8 = 1 << 8
};

enum MusicSkipType_Z {
    MUSIC_SKIP_IMMEDIATE = 0,
    MUSIC_SKIP_MEASURE = 1,
    MUSIC_SKIP_END = 2
};

class VaiMarkerEntry_Z {
public:
    VaiMarkerEntry_Z() {
    }

    ~VaiMarkerEntry_Z() {
    }

    Name_Z m_Name;
    S32 m_Offset;
};

class MusicSkipInfo_Z {
public:
    MusicSkipInfo_Z() {
    }

    ~MusicSkipInfo_Z() {
    }

    Name_Z m_StartMarker;
    Name_Z m_EndMarker;
    MusicSkipType_Z m_Type;
};

class GCSndStream_Z : public BaseStream_Z {
public:
    GCSndStream_Z() {
        m_Voices[1] = NULL;
        m_Voices[0] = NULL;
        m_Unk_0x48 = -1;
    }

    Float SetPan(Float i_Pan, Float i_SurroundPan);
    void SetVolume(Float i_Volume, Float i_Input);
    void SetFrequency(Float i_Frequency);
    void Run();
    void Opened(S32 i_Error, S32 i_Size, S32 i_Offset);
    virtual void Readed(S32 i_Error, S32 i_Size, S32 i_Offset);
    Bool Read(S32 i_Size, S32 i_BufferOffset);
    Bool SetVpb(
        AXVPB* i_Voice,
        SndDatas_Z* i_LeftData,
        SndDatas_Z* i_RightData
    );
    void SetAddr(
        S32 i_VoiceIndex,
        S32 i_EndAddress,
        S32 i_LoopAddress,
        S32 i_CurrentAddress
    );
    S32 Stream(S32 i_Size, S32 i_Offset);
    S32 StreamSwap();
    Bool Process(S32 i_Param);
    void Check();

    U32 m_StreamBufferIndex;
    U32 m_Unk_0x0C[2];
    S32 m_StreamSize;
    S32 m_StreamReadOffset;
    U32 m_Unk_0x1C[3];
    Bool m_StreamBufferNeedsCheck;
    U8 m_Pad_0x29[3];
    S32 m_PendingReadCount;
    S32 m_BufferedSize;
    DynArray_Z<U8, 32, FALSE, FALSE, 32> m_StreamBuffer;
    U32 m_Unk_0x3C;
    AXVPB* m_Voices[2];
    S32 m_Unk_0x48;
    S32 m_StreamWritePosition;
    S32 m_StreamEndPosition;
    U32 m_StreamFlags;
    Bool m_Muted;
    U8 m_Pad_0x59[3];
    Name_Z m_StreamType;
    String_Z<256> m_StreamName;
    DynArray_Z<VaiMarkerEntry_Z, 16, TRUE, TRUE, 4> m_VaiMarkerEntries;
    DynArray_Z<MusicSkipInfo_Z, 16, TRUE, TRUE, 4> m_MusicSkipInfos;
    Float m_ChannelCount;
    U32 m_Unk_0x174[2];
};

class SndEmiter_Z {
public:
    SndEmiter_Z()
        : m_Voice(NULL)
        , m_Volume(1.0f) {
    }

    ~SndEmiter_Z() {
    }

    void* m_Voice;
    Float m_Volume;
    DynArray_Z<U8, 32, FALSE, FALSE, 32> m_SampleData;
    GCSndStream_Z m_SndStream;
};

#endif // _GCSNDSTREAM_Z_H_
