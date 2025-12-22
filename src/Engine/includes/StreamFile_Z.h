#ifndef _STREAMFILE_Z_H_
#define _STREAMFILE_Z_H_
#include "Manipulator_Z.h"
#include "StreamAgent_ZHdl.h"
#include "Streaming_Z.h"
#include "ClassManager_Z.h"
#include "DynPtrArray_Z.h"

class StreamFile_Z : public Manipulator_Z {
public:
    StreamFile_Z() { }

    virtual ~StreamFile_Z();

    Stream_Z& GetStream() { return m_Str; }

    virtual void Init();

    virtual void Update(Float i_DeltaTime);

private:
    S32 m_LoadStage;
    S32 m_LoadStageBeforeResume;
    S32 m_BlockOffsetToRead;
    StreamX_Z m_Str;
    StrBuffer m_Buf[2];
    S32 m_CurBlockIdx;
    BigFileRsc_Z m_CurResource;
    StreamAgent_ZHdl m_AgentHdl;
    Char m_BfPath[ARRAY_CHAR_MAX];
    DynPtrArray_Z<BaseObject_Z*, 32> m_Resources;
    S32 m_CurResourceIdx;
    S32 m_UnkS32_0x174;
    Bool m_RestoreVSync;
};

#endif // _STREAMFILE_Z_H_
