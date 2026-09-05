#ifndef _ANIMATION_Z_H_
#define _ANIMATION_Z_H_
#include "AnimationNode_Z.h"
#include "AnimationMesh_Z.h"
#include "AnimationMaterial_Z.h"
#include "AnimationMorph_Z.h"
#include "ResourceObject_Z.h"
#include "BoneNode_Z.h"

#define FL_ANIM_CCT_NONE 0

typedef DynArray_Z<AnimationNodeKeyId_Z, 16, FALSE, FALSE> AnimationNodeKeyId_ZDA;
typedef DynArray_Z<AnimationMaterialKeyId_Z, 8, FALSE, FALSE> AnimationMaterialKeyId_ZDA;
typedef DynArray_Z<AnimationMeshKeyId_Z, 8, FALSE, FALSE> AnimationMeshKeyId_ZDA;
typedef DynArray_Z<AnimationMorphKeyId_Z, 8, FALSE, FALSE> AnimationMorphKeyId_ZDA;

struct AnimationKeyId_Z {
    AnimationKeyId_Z() {
        m_MeshKeyIds.SetSize(0);
        m_MaterialKeyIds.SetSize(0);
        m_NodeKeyIds.SetSize(0);
        m_MorphKeyIds.SetSize(0);
    }

    AnimationNodeKeyId_ZDA m_NodeKeyIds;
    AnimationMaterialKeyId_ZDA m_MaterialKeyIds;
    AnimationMeshKeyId_ZDA m_MeshKeyIds;
    AnimationMorphKeyId_ZDA m_MorphKeyIds;
    Float m_UnkFloat_0x20;
};

struct AnimationConcat_Z {
    Name_Z m_ObjectName;
    S16 m_ObjectID;
    S16 m_Flag;
    S16 m_StartKfr; // $SABE: Might be a struct together with NbKfr
    S16 m_NbKfr;    // $SABE: Might be a struct together with StartKfr

    inline S16 GetStart() const {
        return m_StartKfr;
    };

    inline S16 GetNb() const {
        return m_NbKfr;
    }
};

struct AnimationConcatNode_Z {
    Name_Z m_BoneName;
    S16 m_BoneID;
    S16 m_Flag;
    U16 m_TransStartKfr;
    U16 m_TransNbKfr;
    U16 m_RotStartKfr;
    U16 m_RotNbKfr;
    U16 m_BezierRotStartKfr;
    U16 m_BezierRotNbKfr;
    U16 m_ScaleStartKfr;
    U16 m_ScaleNbKfr;
    U16 m_MsgStartKfr;
    U16 m_MsgNbKfr;

    inline S16 GetBoneId() const {
        return m_BoneID;
    };

    inline U16 GetMsgStart() const {
        return m_MsgStartKfr;
    }

    inline U16 GetMsgNb() const {
        return m_MsgNbKfr;
    }
};

struct AnimationConcatMtl_Z {
    Name_Z m_MtlName;
    S16 m_MtlID;
    S16 m_Flag;
    S16 m_TransUStartKfr;
    S16 m_TransUNbKfr;
    S16 m_EmissiveColorStartKfr;
    S16 m_EmissiveColorNbKfr;
    S16 m_DiffuseColorStartKfr;
    S16 m_DiffuseColorNbKfr;
    S16 m_RotStartKfr;
    S16 m_RotNbKfr;
    S16 m_TransVStartKfr;
    S16 m_TransVNbKfr;
};

typedef DynArray_Z<AnimationConcat_Z, 32, FALSE, FALSE> AnimationConcat_ZDA;
typedef DynArray_Z<AnimationConcatNode_Z, 32, FALSE, FALSE> AnimationConcatNode_ZDA;
typedef DynArray_Z<AnimationConcatMtl_Z, 32, FALSE, FALSE> AnimationConcatMtl_ZDA;

class Animation_Z : public ResourceObject_Z {
public:
    Animation_Z() { }

    // clang-format off
    virtual ~Animation_Z() { }
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void EndLoadLinks();
    virtual void Clean();
    virtual Bool MarkHandles();
    // clang-format on

    static BaseObject_Z* NewObject() {
        return NewL_Z(102) Animation_Z;
    }

    S32 GetIndexOfNodeById(S32 Id) const;
    Float GetTimeFromMessage(S32 i_MsgId, S32 i_NodeId, Float i_StartTime = 0.f);
    Bool GetMessageFromNode(S32 i_NodeId, Float i_PrecTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Msg, S32& io_NbMsg);
    void BuildSmooth(
        Float i_Time,
        Float i_SmoothTime,
        const BoneNodePtr_ZDA& i_OriginalBones,
        const BoneNodePtr_ZDA& i_InstantiatedBones,
        const AnimationNodeModifier_ZDA& i_Modifiers,
        AnimationConcatNode_ZDA& i_Ccts,
        AnimationNodeData_Z& i_Data
    );
    void GetNodeCct(const Vec3f& i_Times, const BoneNodePtr_ZDA& i_OriginalBones, const BoneNodePtr_ZDA& i_InstantiatedBones, const AnimationNodeModifier_ZDA& i_Modifiers);
    static void GetExtNodeCct(
        const Vec3f& i_Times,
        const BoneNodePtr_ZDA& i_OriginalBones,
        const BoneNodePtr_ZDA& i_InstantiatedBones,
        const AnimationNodeModifier_ZDA& i_Modifiers,
        const AnimationConcatNode_ZDA& i_Ccts,
        const AnimationNodeData_Z& i_Data
    );

    inline Float GetMaxTime() const { return m_MaxTime; }

    inline Float GetSmoothRatio() const { return m_SmoothRatio; }

    inline U16 GetFlag() const { return m_Flag; }

    inline int GetNbAnimationNode() const {
        return m_NodeCct.GetSize();
    }

    inline const AnimationConcatNode_ZDA& GetCctNodes() const {
        return m_NodeCct;
    }

    inline const AnimationNodeData_Z& GetKfrNode() const {
        return m_NodeKfr;
    }

    inline int GetNbAnimationMtl() const {
        return m_MtlCct.GetSize();
    }

    inline const AnimationConcatMtl_ZDA& GetCctMtls() const {
        return m_MtlCct;
    }

    inline const AnimationMtlData_Z& GetKfrMtl() const {
        return m_MtlKfr;
    }

    inline int GetNbAnimationMesh() const {
        return m_MeshCct.GetSize();
    }

    inline const AnimationConcat_ZDA& GetCctMeshes() const {
        return m_MeshCct;
    }

    inline const AnimationMeshData_Z& GetKfrMesh() const {
        return m_MeshKfr;
    }

    inline int GetNbAnimationMorph() const {
        return m_MorphCct.GetSize();
    }

    inline const AnimationConcat_ZDA& GetCctMorphs() const {
        return m_MorphCct;
    }

    inline const AnimationMorphData_Z& GetKfrMorph() const {
        return m_MorphKfr;
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
