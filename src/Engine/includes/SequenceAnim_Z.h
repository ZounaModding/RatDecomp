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
    virtual ~SequenceAnim_Z();                            /* 0x08 */
    virtual void ResetSeqMan(S32 a1, Bool a2);            /* 0x0C */
    virtual void Next(S32 a1);                            /* 0x10 */
    virtual void WarnNext(S32 a1);                        /* 0x14 */
    virtual void SetSeq(const SeqAnim_Z& a1, S32 a2);     /* 0x18 */
    virtual void SetNextSeq(const SeqAnim_Z& a1, S32 a2); /* 0x1C */
    virtual void AddSeq(const SeqAnim_Z& a1, S32 a2);     /* 0x20 */
    virtual void GetNbRsc();                              /* 0x24 */
};

#endif // _SEQUENCEANIM_Z_H_
