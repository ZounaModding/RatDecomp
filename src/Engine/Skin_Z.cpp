#include "Skin_Z.h"
#include "AnimationManager_Z.h"
#include "ClassManager_Z.h"
#include "SoundManager_Z.h"

Skin_Z::Skin_Z() {
    SetGeometryType(SKIN_Z);
    EnableFlag(FL_IS_SKIN_UNK_0x100000);
}

Skin_Z::~Skin_Z() {
    Name_Z& l_RscName = GetName();
    Name_Z& l_RscFileName = gData.ClassMgr->GetHandleName(GetHandle());
    if (gData.AnimMgr) {
        gData.AnimMgr->RemoveClassId(l_RscName, l_RscFileName);
    }
    if (gData.SoundMgr) {
        gData.SoundMgr->RemoveClassId(l_RscName, l_RscFileName);
    }
}

Bool Skin_Z::MarkHandles() {
    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }

    for (S32 i = 0; i < m_ObjectHdls.GetSize(); i++) {
        gData.ClassMgr->MarkHandles(m_ObjectHdls[i]);
    }

    Name_Z& l_RscFileName = gData.ClassMgr->GetHandleName(GetHandle());

    if (gData.AnimMgr) {
        gData.AnimMgr->MarkHandlesFromClass(GetName(), l_RscFileName);
    }

    if (gData.SoundMgr) {
        gData.SoundMgr->MarkHandlesFromClass(GetName(), l_RscFileName);
    }

    return TRUE;
}

void Skin_Z::Clean() {
    Object_Z::Clean();
    m_Bones.Empty();
    m_ObjectHdls.Empty();
    m_RotshapeObjects.Empty();
}
