#ifndef _ANIMATION_Z_H_
#define _ANIMATION_Z_H_
#include "AnimationNode_Z.h"
#include "AnimationMesh_Z.h"
#include "AnimationMaterial_Z.h"
#include "AnimationMorph_Z.h"
#include "ResourceObject_Z.h"

struct AnimationConcat_Z {
    Name_Z m_ObjectName;
    S16 m_ObjectID;
    S16 m_Flag;
    S16 m_StartKfr; // $SABE: Might be a struct together with EndKfr
    S16 m_NbKfr;    // $SABE: Might be a struct together with StartKfr
};

struct AnimationConcatNode_Z {
    Name_Z m_BoneName;
    S16 m_BoneID;
    S16 m_Flag;
    S16 m_TransStartKfr;
    S16 m_TransEndKfr;
    S16 m_RotStartKfr;
    S16 m_RotEndKfr;
    S16 m_BezierRotStartKfr;
    S16 m_BezierRotEndKfr;
    S16 m_ScaleStartKfr;
    S16 m_ScaleEndKfr;
    S16 m_MsgStartKfr;
    S16 m_MsgEndKfr;
};

struct AnimationConcatMtl_Z {
    Name_Z m_MtlName;
    S16 m_MtlID;
    S16 m_Flag;
    S16 m_TransUStartKfr;
    S16 m_TransUEndKfr;
    S16 m_TransVStartKfr;
    S16 m_TransVEndKfr;
    S16 m_DiffuseColorStartKfr;
    S16 m_DiffuseColorEndKfr;
    S16 m_EmissiveColorStartKfr;
    S16 m_EmissiveColorEndKfr;
    S16 m_RotStartKfr;
    S16 m_RotEndKfr;
};

typedef DynArray_Z<AnimationConcat_Z, 32, 0, 0> AnimationConcat_ZDA;
typedef DynArray_Z<AnimationConcatNode_Z, 32, 0, 0> AnimationConcatNode_ZDA;
typedef DynArray_Z<AnimationConcatMtl_Z, 32, 0, 0> AnimationConcatMtl_ZDA;

class Animation_Z : public ResourceObject_Z {
public:
    Animation_Z();
    virtual ~Animation_Z();           /* 0x08 */
    virtual void Load(void** i_Data); /* 0x0C */
    virtual void EndLoad();           /* 0x10 */
    virtual void AfterEndLoad();      /* 0x14 */
    virtual void EndLoadLinks();      /* 0x18 */
    virtual void Clean();             /* 0x1C */
    virtual Bool MarkHandles();       /* 0x20 */

    inline Float GetMaxTime() const { return m_MaxTime; }

    inline Float GetSmoothRatio() const { return m_SmoothRatio; }

    inline U16 GetFlag() const { return m_Flag; }

    inline U32 GetNbAnimationNode() const {
        return m_NodeCct.GetSize();
    }

    inline const AnimationConcatNode_ZDA& GetCctNodes() const {
        return m_NodeCct;
    }

    inline const AnimationNodeData_Z& GetKfrNode() const {
        return m_NodeKfr;
    }

    inline U32 GetNbAnimationMtl() const {
        return m_MtlCct.GetSize();
    }

    inline const AnimationConcatMtl_ZDA& GetCctMtls() const {
        return m_MtlCct;
    }

    inline const AnimationMtlData_Z& GetKfrMtl() const {
        return m_MtlKfr;
    }

    inline U32 GetNbAnimationMesh() const {
        return m_MeshCct.GetSize();
    }

    inline const AnimationConcat_ZDA& GetCctMeshes() const {
        return m_MeshCct;
    }

    inline const AnimationMeshData_Z& GetKfrMesh() const {
        return m_MeshKfr;
    }

    inline U32 GetNbAnimationMorph() const {
        return m_MorphCct.GetSize();
    }

    inline const AnimationConcat_ZDA& GetCctMorphs() const {
        return m_MorphCct;
    }

    inline const AnimationMorphData_Z& GetKfrMorph() const {
        return m_MorphKfr;
    }

    static BaseObject_Z* NewObject() {
        return New_Z Animation_Z;
    }

private:
    Float m_MaxTime;
    Float m_SmoothRatio;
    U16 m_Flag;
    AnimationConcatNode_ZDA m_NodeCct;
    AnimationConcatMtl_ZDA m_MtlCct;
    AnimationConcat_ZDA m_MeshCct;
    AnimationConcat_ZDA m_MorphCct;
    AnimationNodeData_Z m_NodeKfr;
    AnimationMeshData_Z m_MeshKfr;
    AnimationMtlData_Z m_MtlKfr;
    AnimationMorphData_Z m_MorphKfr;
};

#endif // _ANIMATION_Z_H_
