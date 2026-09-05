#include "Lod_Z.h"
#include "ClassManager_Z.h"
#include "AnimationManager_Z.h"
#include "Memory_Z.h"
#include "SoundManager_Z.h"
#include "UserDefine_Z.h"

Lod_Z::Lod_Z() {
    SetGeometryType(LOD_Z);
    m_ShadowSphere = NULL;
    Clean();
}

Lod_Z::~Lod_Z() {
}

void Lod_Z::Clean() {
    Object_Z::Clean();
    m_SphereCollisions.Empty();
    m_BoxCollisions.Empty();
    m_CylindreCollisions.Empty();
    if (m_ShadowSphere) {
        Delete_Z m_ShadowSphere;
    }

    m_ShadowSphere = NULL;
    m_LodDistanceScale = 1.0f;
    m_NearFadeThreshold = 0.0f;
    m_FarFadeThreshold = 0.0f;
    m_UserDefine = NULL;
    m_ObjectHdls.Empty();
    m_VolumeObjectHdl = HANDLE_NULL;
}

Bool Lod_Z::MarkHandles() {
    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }

    for (S32 i = 0; i < m_ObjectHdls.GetSize(); i++) {
        gData.ClassMgr->MarkHandles(m_ObjectHdls[i]);
    }

    gData.ClassMgr->MarkHandles(m_VolumeObjectHdl);

    Name_Z& l_RscName = gData.ClassMgr->GetHandleName(GetHandle());

    if (gData.AnimMgr) {
        gData.AnimMgr->MarkHandlesFromClass(GetName(), l_RscName);
    }

    if (gData.SoundMgr) {
        gData.SoundMgr->MarkHandlesFromClass(GetName(), l_RscName);
    }

    if (m_UserDefine) {
        m_UserDefine->MarkHandles();
    }

    return TRUE;
}

LodData_Z::LodData_Z() {
    m_ActorData = NULL;
}

LodData_Z::~LodData_Z() {
}

Bool LodData_Z::MarkHandles() {
    if (!ObjectDatas_Z::MarkHandles()) {
        return FALSE;
    }

    for (S32 i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
        if (m_ObjectDatasHdls[i].IsValid()) {
            m_ObjectDatasHdls[i]->MarkHandles();
        }
    }

    if (m_VolumeObjectDatasHdl.IsValid()) {
        m_VolumeObjectDatasHdl->MarkHandles();
    }

    return TRUE;
}

// TODO: Finish matching
void LodData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Name_Z l_ClassName(Name_Z::GetID("LodData_Z", 0));

    o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, GetName());

    ObjectDatas_ZHdl l_DataHdl;

    l_DataHdl = o_ObjectDatasHdl;

    LodData_Z* l_Data = (LodData_Z*)GETPTR(l_DataHdl);

    ObjectDatas_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);

    Object_ZHdl l_ObjectHdl;

    l_ObjectHdl = o_ObjectHdl;

    Lod_Z* l_Lod = (Lod_Z*)GETPTR(l_ObjectHdl);
    S32 l_Nb = m_ObjectDatasHdls.GetSize();

    if (l_Nb) {
        l_Data->m_ObjectDatasHdls.SetSize(l_Nb);

        if (m_ObjectDatasHdls[0].IsValid()) {
            m_ObjectDatasHdls[0]->Clone(l_Data->m_ObjectDatasHdls[0], l_Lod->GetObjectA(0));
        }
    }

    for (S32 i = 1; i < m_ObjectDatasHdls.GetSize(); i++) {
        if (m_ObjectDatasHdls[i] == m_ObjectDatasHdls[i - 1]) {
            l_Data->m_ObjectDatasHdls[i] = l_Data->m_ObjectDatasHdls[i - 1];
        }
        else if (m_ObjectDatasHdls[i].IsValid()) {
            m_ObjectDatasHdls[i]->Clone(l_Data->m_ObjectDatasHdls[i], l_Lod->GetObjectA(i));
        }
    }

    if (m_VolumeObjectDatasHdl.IsValid()) {
        S32 j;

        for (j = 0; j < m_ObjectDatasHdls.GetSize(); j++) {
            if (m_ObjectDatasHdls[j] == m_VolumeObjectDatasHdl) {
                l_Data->m_VolumeObjectDatasHdl = l_Data->m_ObjectDatasHdls[j];
                break;
            }
        }

        if (j == m_ObjectDatasHdls.GetSize()) {
            m_VolumeObjectDatasHdl->Clone(l_Data->m_VolumeObjectDatasHdl, l_Lod->GetVolume());
        }
    }
    l_Data->SetActorData(l_Lod);
}

void LodData_Z::SetActorData(Lod_Z* i_Lod) {
    if (i_Lod->IsFlag(FL_IS_LOD_ANIMATED)) {
        EnableFlag(FL_OBJECTDATAS_DISPLAY);

        U32 l_Flag = i_Lod->GetFlag();

        if ((l_Flag & FL_IS_LOD_UNK_0x400000) && (l_Flag & FL_IS_LOD_UNK_0x800000)) {
            return;
        }

        m_ActorData = NewL_Z(462) ActorData_Z(i_Lod->GetFlag());
    }
}

Bool Lod_Z::GetWorldShadowSphere(Node_Z* i_Node, Sphere_Z& o_Sphere) const {
    return FALSE;
}
