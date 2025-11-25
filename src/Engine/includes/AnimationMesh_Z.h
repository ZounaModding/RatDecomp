#ifndef _ANIMATIONMESH_Z_H_
#define _ANIMATIONMESH_Z_H_
#include "Keyframer_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"

class Skel_Z;
class AnimationConcat_Z;

struct AnimationMeshKeyId_Z {
    AnimationMeshKeyId_Z() {
        Reset();
    }

    void Reset() {
        m_CurVisibility = 1;
    }

    S16 m_CurVisibility;
};

struct AnimationMeshData_Z {
    KeyframerFloatComp_Z m_VisibilityKfr;
};

class AnimationMesh_Z {
private:
    Name_Z m_ObjectName;
    int m_ObjectId;
    AnimationMeshData_Z m_Data;

public:
    static void UpdateCct(Float i_Time, const AnimationMeshData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, AnimationMeshKeyId_Z& i_KeyId, Skel_Z* i_Skel);
    static void EndCct(Float, const AnimationMeshData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, Skel_Z* i_Skel);
    static void Load(AnimationMeshData_Z& i_AnimData, void** i_Data);
};
#endif // _ANIMATIONMESH_Z_H_
