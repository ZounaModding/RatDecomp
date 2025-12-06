#ifndef _ANIMATIONMORPH_Z_H_
#define _ANIMATIONMORPH_Z_H_
#include "Keyframer_Z.h"
#include "MeshMorph_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"

class AnimationConcat_Z;
class MorphTarget_Z;

struct AnimationMorphKeyId_Z {
    S16 m_CurMorph;
    AnimationMorphKeyId_Z();
    void Reset();

    inline S16 GetCurMorph() const {
        return m_CurMorph;
    }

    inline void SetCurMorph(S16 i_CurMorph) {
        m_CurMorph = i_CurMorph;
    }
};

struct AnimationMorphData_Z {
    KeyframerFloatComp_Z m_MorphKfr;

    inline void Flush() {
        m_MorphKfr.Flush();
    }
};

class AnimationMorph_Z {
private:
    Name_Z m_ObjectName;
    int m_ObjectId;
    AnimationMorphData_Z m_Data;

public:
    static void UpdateCct(Float i_Time, const AnimationMorphData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, AnimationMorphKeyId_Z& i_KeyId, MorphTarget_Z& i_MorphTarget);
    static void Load(AnimationMorphData_Z& i_AnimData, void** i_Data);
};
#endif // _ANIMATIONMORPH_Z_H_
