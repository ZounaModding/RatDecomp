#ifndef _ANIMATIONMANAGER_Z_H_
#define _ANIMATIONMANAGER_Z_H_
#include "ClassNameResManager_Z.h"
#include "DynPtrArray_Z.h"
#include "AnimMessage_Z.h"

class BoneNode_Z;
class RegMessage_Z;
class SkelMessage_Z;

#define ANIM_RESOURCE_MAX 128

enum NodeIds {
    BONE_TETE = 0,
    BONE_POIDRO,
    BONE_POIGAU,
    BONE_COUDRO,
    BONE_COUGAU,
    BONE_EPADRO,
    BONE_EPAGAU,
    BONE_HANDRO,
    BONE_HANGAU,
    BONE_GENDRO,
    BONE_GENGAU,
    BONE_PIEDRO,
    BONE_PIEGAU,
    BONE_BASDOS,
    BONE_SELLE,
    BONE_SOL,
    BONE_TREE,
    BONE_LINK,
    BONE_MOVE,
    BONE_CAMERA,
    BONE_COU,
    BONE_MILDOS,
    BONE_HAUDOS,
    BONE_DDPGAU,
    BONE_DDPDRO,
    BONE_HIPS,
    BONE_TRONCHE,
    BONE_TRONCHE1,
    BONE_TRONCHE2,
    BONE_TRONCHE3,
    BONE_TRONCHE4,
    BONE_TRONCHE5,
    BONE_CERVO,
    BONE_DOIDRO3,
    BONE_ARME01,
    BONE_ARME2,
    BONE_ARME05,
    BONE_ECLAIR,
    BONE_DYN1,
    BONE_DYN2,
    BONE_DYN3,
    BONE_DYN4,
    BONE_DYN5,
    BONE_DYN6,
    BONE_DYN7,
    BONE_DYN8,
    BONE_DYN9,
    BONE_DYN10,
    BONE_DYN11,
    BONE_DYN12,
    BONE_DYN13,
    BONE_DYN14,
    BONE_DYN15,
    BONE_DYN16,
    BONE_HIDE_01,
    BONE_HIDE_02,
    BONE_HIDE_03,
    BONE_HIDE_04,
    BONE_HIDE_05,
    BONE_HIDE_06,
    BONE_HIDE_07,
    BONE_HIDE_08,
    BONE_HIDE_09,
    BONE_HIDE_10,
    BONE_HIDE_11,
    BONE_HIDE_12,
    BONE_HIDE_13,
    BONE_HIDE_14,
    BONE_HIDE_15,
    BONE_HIDE_16
};

enum MaterialIds {
    MAT_SKIN = 0,
    MAT_SHOES,
    MAT_CLOTHE,
    MAT_HAIRS,
    MAT_MOUTH,
    MAT_FACE
};

struct SkelNodeArray {
    DynPtrArray_Z<BoneNode_Z*, 4> m_SkelNodes;
    BoneNode_Z* m_BonesArrayPtr;
    BoneNode_Z* m_RootBonePtr;
    Name_Z m_SkelName;
    int m_RefCount;

    // $SABE TODO: Fix
    SkelNodeArray() {
        m_BonesArrayPtr = NULL;
        m_RootBonePtr = NULL;
        m_RefCount = 0;
    }

    // $SABE TODO: Fix
    ~SkelNodeArray() {
        if (m_BonesArrayPtr) {
            Free_Z(m_BonesArrayPtr);
        }
        else {
            ASSERTLE_Z(m_SkelNodes.GetSize() == 0 && m_BonesArrayPtr == NULL, "", 21, "SkelNodes.GetSize()==0&&BonesArrayPtr==0");
        }
        m_SkelNodes.Empty();
        m_BonesArrayPtr = NULL;
        m_RootBonePtr = NULL;
    }
};

class AnimationManager_Z : public ClassNameResManager_Z {
public:
    AnimationManager_Z();

    virtual Bool MarkHandles();
    virtual Bool Minimize();
    virtual ~AnimationManager_Z();

    void SetNodeId(const Name_Z& i_Name, S32 i_Id);
    void SetMaterialId(const Name_Z& i_Name, S32 i_Id);
    U32 GetNodeByName(const Name_Z& i_Name);
    U32 GetMaterialByName(const Name_Z& i_Name);
    U32 GetMeshByName(const Name_Z& i_Name);
    void ReserveMsgArray();

private:
    HashName_ZTable_Z m_NodeIds;
    DynArray_Z<Name_Z, 32, FALSE> m_NodeNames;
    HashName_ZTable_Z m_MaterialIds;
    DynArray_Z<Name_Z, 32, FALSE> m_MaterialNames;
    HashName_ZTable_Z m_MeshIds;
    DynArray_Z<Name_Z, 32, FALSE> m_MeshNames;
    HoleArray_Z<SkelNodeArray> m_SkelOriginalNodes;
    RegMessage_Z* m_AnimMsg;
    RegMessage_Z* m_BufAnimMsg;
    U32 m_AnimMsgNb;
    U32 m_BufAnimMsgNb;
    SkelMessage_Z* m_SkelMsg;
    SkelMessage_Z* m_BufSkelMsg;
    U32 m_SkelMsgNb;
    U32 m_BufSkelMsgNb;
    U32 m_UnkU32_0xC4;
    RegMessage_Z m_DfltRegMsg;
    SkelMessage_Z m_DfltSkelMsg;
    U32 m_UpdateFlag;
};

#endif
