#ifndef _SEQUENCEMIXANIM_Z_H_
#define _SEQUENCEMIXANIM_Z_H_
#include "SequenceAnim_Z.h"

class SequenceMixAnim_Z : public SequenceAnim_Z {
public:
    virtual ~SequenceMixAnim_Z() { }

    virtual void ResetSeqMan(S32 a1, bool a2);
    virtual void Next(S32 a1);
    virtual void WarnNext(S32 a1);
    virtual void SetSeq(const SeqAnim_Z& a1, S32 a2);
    virtual void SetNextSeq(const SeqAnim_Z& a1, S32 a2);
    virtual void AddSeq(const SeqAnim_Z& a1, S32 a2);

    virtual void GetNbRsc() { }

    static BaseObject_Z* NewObject() {
        return NewL_Z(35) SequenceMixAnim_Z;
    }
};

#endif // _SEQUENCEMIXANIM_Z_H_
