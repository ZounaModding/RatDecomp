#ifndef _SEQUENCEANIM_Z_H_
#define _SEQUENCEANIM_Z_H_
#include "BaseObject_Z.h"
#include "Animation_ZHdl.h"

struct SeqAnim_Z {
    Animation_ZHdl m_AnimHdl;
    abc_message m_EndMessage;
    Float m_UnkFloat_0x8;
    Float m_SmoothTime;
    Float m_UnkFloatOne_0x10;
    Float m_CurTime;
    Bool m_Smooth;
    Bool m_UnkBoolFalse_0x19;
    Float m_UnkFloatOne_0x1c;
};

class SequenceAnim_Z : public BaseObject_Z {
public:
    virtual ~SequenceAnim_Z() { }

    virtual void ResetSeqMan(S32 a1, Bool a2);
    virtual void Next(S32 a1);
    virtual void WarnNext(S32 a1);
    virtual void SetSeq(const SeqAnim_Z& a1, S32 a2);
    virtual void SetNextSeq(const SeqAnim_Z& a1, S32 a2);
    virtual void AddSeq(const SeqAnim_Z& a1, S32 a2);

    virtual void GetNbRsc() { }

    static BaseObject_Z* NewObject() {
        return NewL_Z(41) SequenceAnim_Z;
    }
};

#endif // _SEQUENCEANIM_Z_H_
