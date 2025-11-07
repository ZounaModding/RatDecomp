#include "AnimationManager_Z.h"
#include "String_Z.h"

Extern_Z "C" int sprintf(char* i_Buf, const char* i_Format, ...);

AnimationManager_Z::AnimationManager_Z()
    : m_NodeIds(ANIM_RESOURCE_MAX)
    , m_MaterialIds(ANIM_RESOURCE_MAX)
    , m_MeshIds(ANIM_RESOURCE_MAX) {
    SetNodeId(Name_Z::GetID("TETE"), BONE_TETE);
    SetNodeId(Name_Z::GetID("POIDRO"), BONE_POIDRO);
    SetNodeId(Name_Z::GetID("POIGAU"), BONE_POIGAU);
    SetNodeId(Name_Z::GetID("COUDRO"), BONE_COUDRO);
    SetNodeId(Name_Z::GetID("COUGAU"), BONE_COUGAU);
    SetNodeId(Name_Z::GetID("EPADRO"), BONE_EPADRO);
    SetNodeId(Name_Z::GetID("EPAGAU"), BONE_EPAGAU);
    SetNodeId(Name_Z::GetID("HANDRO"), BONE_HANDRO);
    SetNodeId(Name_Z::GetID("HANGAU"), BONE_HANGAU);
    SetNodeId(Name_Z::GetID("GENDRO"), BONE_GENDRO);
    SetNodeId(Name_Z::GetID("GENGAU"), BONE_GENGAU);
    SetNodeId(Name_Z::GetID("PIEDRO"), BONE_PIEDRO);
    SetNodeId(Name_Z::GetID("PIEGAU"), BONE_PIEGAU);
    SetNodeId(Name_Z::GetID("BASDOS"), BONE_BASDOS);
    SetNodeId(Name_Z::GetID("SELLE"), BONE_SELLE);
    SetNodeId(Name_Z::GetID("SOL"), BONE_SOL);
    SetNodeId(Name_Z::GetID("TREE"), BONE_TREE);
    SetNodeId(Name_Z::GetID("LINK"), BONE_LINK);
    SetNodeId(Name_Z::GetID("MOVE"), BONE_MOVE);
    SetNodeId(Name_Z::GetID("CAMERA"), BONE_CAMERA);
    SetNodeId(Name_Z::GetID("COU"), BONE_COU);
    SetNodeId(Name_Z::GetID("MILDOS"), BONE_MILDOS);
    SetNodeId(Name_Z::GetID("HAUDOS"), BONE_HAUDOS);
    SetNodeId(Name_Z::GetID("DDPGAU"), BONE_DDPGAU);
    SetNodeId(Name_Z::GetID("DDPDRO"), BONE_DDPDRO);
    SetNodeId(Name_Z::GetID("HIPS"), BONE_HIPS);
    SetNodeId(Name_Z::GetID("TRONCHE"), BONE_TRONCHE);
    SetNodeId(Name_Z::GetID("TRONCHE1"), BONE_TRONCHE1);
    SetNodeId(Name_Z::GetID("TRONCHE2"), BONE_TRONCHE2);
    SetNodeId(Name_Z::GetID("TRONCHE3"), BONE_TRONCHE3);
    SetNodeId(Name_Z::GetID("TRONCHE4"), BONE_TRONCHE4);
    SetNodeId(Name_Z::GetID("TRONCHE5"), BONE_TRONCHE5);
    SetNodeId(Name_Z::GetID("CERVO"), BONE_CERVO);
    SetNodeId(Name_Z::GetID("DOIDRO3"), BONE_DOIDRO3);
    SetNodeId(Name_Z::GetID("ARME01"), BONE_ARME01);
    SetNodeId(Name_Z::GetID("ARME2"), BONE_ARME2);
    SetNodeId(Name_Z::GetID("ARME05"), BONE_ARME05);
    SetNodeId(Name_Z::GetID("ECLAIR"), BONE_ECLAIR);
    String_Z<ARRAY_CHAR_MAX> l_DynNames;
    for (S32 i = 0; i < 16; i++) {
        sprintf(l_DynNames, "DYN%d", i + 1);
        SetNodeId(Name_Z::GetID(l_DynNames), BONE_DYN1 + i);
    }
    for (S32 i = 0; i < 16; i++) {
        sprintf(l_DynNames, "HIDE_%02d", i + 1);
        SetNodeId(Name_Z::GetID(l_DynNames), BONE_HIDE_01 + i);
    }
    SetMaterialId(Name_Z::GetID("SKIN"), MAT_SKIN);
    SetMaterialId(Name_Z::GetID("SHOES"), MAT_SHOES);
    SetMaterialId(Name_Z::GetID("CLOTHE"), MAT_CLOTHE);
    SetMaterialId(Name_Z::GetID("HAIRS"), MAT_HAIRS);
    SetMaterialId(Name_Z::GetID("MOUTH"), MAT_MOUTH);
    SetMaterialId(Name_Z::GetID("FACE"), MAT_FACE);
    ReserveMsgArray();
}

Bool AnimationManager_Z::MarkHandles() {
    ClassNameResManager_Z::MarkHandles();
    return TRUE;
}

Bool AnimationManager_Z::Minimize() {
    ClassNameResManager_Z::Minimize();
    return TRUE;
}

AnimationManager_Z::~AnimationManager_Z() {
}

void AnimationManager_Z::SetNodeId(const Name_Z& i_Name, S32 i_Id) {
}
