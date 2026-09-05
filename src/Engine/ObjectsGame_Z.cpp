#include "ObjectsGame_Z.h"
#include "Game_Z.h"
#include "ObjectMove_Z.h"
#include "ObjectMoveCollFlag_Z.h"
#include "Particles_Z.h"
#include "PlayParticles_Z.h"
#include "SeadHandle_Z.h"
#include "World_Z.h"

void ObjectsGame_Z::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_game_manager);
    SetNbVp(0);
    SetFirstVp(-1);
    SetGroup(ag_collision);
    m_IsRunningUpdate = FALSE;
    m_IsRunningUpdateFromManager = FALSE;
}

void ObjectsGame_Z::ActionOnDeactivate() {
}

void ObjectsGame_Z::ActionOnActivate() {
    ObjectGame_Z::ActionOnActivate();
    S32 i;
    S32 l_NbObject = m_ObjectMoveGameDA.GetSize();
    for (i = 0; i < l_NbObject; i++) {
        ObjectMove_Z* l_ObjectMove = m_ObjectMoveGameDA[i].m_ObjectMove;
        if (l_ObjectMove->IsFlagEnable(COL_BOL_VOLUME)) {
            Game_Z* l_Game = GetGameHdl();
            World_Z* l_World = l_Game->GetWorld();
            l_ObjectMove->UpdateCollisionVolume(l_World, TRUE);
        }
    }
}

void ObjectsGame_Z::Reset() {
}

void ObjectsGame_Z::AddGameObject(const ObjectMove_ZHdl& a1) {
}

void ObjectsGame_Z::RemoveGameObject(const ObjectMove_ZHdl& a1) {
}

void ObjectsGame_Z::AddGamePlayParticles(const PlayParticles_ZHdl& i_PlayParticlesHdl) {
    PlayParticles_Z* l_PlayParticles = i_PlayParticlesHdl;
    S32 i;
    for (i = 0; i < m_PlayParticlesGameDA.GetSize(); i++) {
        if (m_PlayParticlesGameDA[i].m_PlayParticles == l_PlayParticles) {
            break;
        }
    }
    if (i == m_PlayParticlesGameDA.GetSize()) {
        Node_Z* l_Node = l_PlayParticles->GetNode();
        Particles_Z* l_Particles = (Particles_Z*)l_Node->GetObject();
        Agent_Z* l_Agent = l_Node->GetAgent();
        m_PlayParticlesGameDA.Add(PlayParticlesGame_Z(l_PlayParticles, l_Node, l_Agent, l_Particles, l_Node->GetName()));
    }
}

void ObjectsGame_Z::UnregistredFromGame(const Agent_ZHdl& a1) { }

void ObjectsGame_Z::Update(Float i_DeltaTime) {
    m_IsRunningUpdate = TRUE;
    UpdateObject(i_DeltaTime);
    UpdateSkelCollision();
    m_IsRunningUpdate = FALSE;
}

// TODO: Finish matching
void ObjectsGame_Z::UpdateBuffer() {
    S32 i;
    for (i = 0; i < m_AddedDuringUpdateObjectMoveHdls.GetSize(); i++) {
        AddGameObject(m_AddedDuringUpdateObjectMoveHdls[i]);
    }
    S32 i_AddParticles;
    for (i_AddParticles = 0; i_AddParticles < m_AddedDuringUpdatePlayParticlesHdls.GetSize(); i_AddParticles++) {
        const PlayParticles_ZHdl& l_PlayParticlesHdl = m_AddedDuringUpdatePlayParticlesHdls[i_AddParticles];
        PlayParticles_Z* l_PlayParticles = l_PlayParticlesHdl;
        S32 j;
        j = 0;
        while (j < m_PlayParticlesGameDA.GetSize()) {
            if (m_PlayParticlesGameDA[j].m_PlayParticles == l_PlayParticles) {
                break;
            }
            j++;
        }
        if (j == m_PlayParticlesGameDA.GetSize()) {
            Node_Z* l_Node = l_PlayParticles->GetNode();
            Particles_Z* l_Particles = (Particles_Z*)l_Node->GetObject();
            Agent_Z* l_Agent = l_Node->GetAgent();
            m_PlayParticlesGameDA.Add(PlayParticlesGame_Z(l_PlayParticles, l_Node, l_Agent, l_Particles, l_Node->GetName()));
        }
    }

    m_AddedDuringUpdateObjectMoveHdls.Empty();
    m_AddedDuringUpdatePlayParticlesHdls.Empty();

    for (i = 0; i < m_RemovedDuringUpdateObjectMoveHdls.GetSize(); i++) {
        RemoveGameObject(m_RemovedDuringUpdateObjectMoveHdls[i]);
    }
    for (i = 0; i < m_RemovedDuringUpdatePlayParticlesHdls.GetSize(); i++) {
        const PlayParticles_ZHdl& l_PlayParticlesHdl = m_RemovedDuringUpdatePlayParticlesHdls[i];
        PlayParticles_Z* l_PlayParticles = l_PlayParticlesHdl;
        S32 j;
        j = 0;
        while (j < m_PlayParticlesGameDA.GetSize()) {
            if (m_PlayParticlesGameDA[j].m_PlayParticles == l_PlayParticles) {
                break;
            }
            j++;
        }
        if (j != m_PlayParticlesGameDA.GetSize()) {
            m_PlayParticlesGameDA.Remove(j);
            l_PlayParticlesHdl->Deactivate();
        }
    }
    m_RemovedDuringUpdateObjectMoveHdls.Empty();
    m_RemovedDuringUpdatePlayParticlesHdls.Empty();
}

void ObjectsGame_Z::UpdateObject(Float i_DeltaTime) {
    Game_Z* l_Game = GetGameHdl();
    World_Z* l_World = l_Game->GetWorld();
    S32 i = m_ObjectMoveGameDA.GetSize() - 1;
    ObjectMoveGame_Z* l_ObjectMoveGame = m_ObjectMoveGameDA.GetArrayPtr() + i;
    Vec2f l_BMin;
    Vec2f l_BMax;
    Vec3f l_CurPos;
    Vec3f l_NewPos;
    Vec3f l_Orient;
    Vec3f l_LookAt;
    SeadZone_Z l_SeadZone;
    SeadZoneData_Z l_SeadZoneDatas[SEAD_RECURSE_MAX_NODE];
    l_SeadZone.SetWorld(l_World);

    for (; i >= 0; i--, l_ObjectMoveGame--) {
        ObjectMove_Z* l_ObjectMove = l_ObjectMoveGame->m_ObjectMove;
        if (l_ObjectMove->IsActive()) {
            l_CurPos = l_ObjectMove->GetPos(0);
            if (l_ObjectMove->IsFlagEnable(COL_BOL_ABORTED)) {
                l_ObjectMove->ComputeNewPos(l_CurPos, l_NewPos, l_Orient, 0.0f, 0);
            }
            else {
                l_ObjectMove->ComputeNewPos(l_CurPos, l_NewPos, l_Orient, i_DeltaTime, 0);
            }
            l_LookAt = l_NewPos + l_Orient;
            l_ObjectMove->DisableCollision(l_World, i_DeltaTime, 0);

            if (l_CurPos.x > l_NewPos.x) {
                l_BMin.x = l_NewPos.x - 1.0f;
                l_BMax.x = l_CurPos.x + 1.0f;
            }
            else {
                l_BMax.x = l_NewPos.x + 1.0f;
                l_BMin.x = l_CurPos.x - 1.0f;
            }
            if (l_CurPos.z > l_NewPos.z) {
                l_BMin.y = l_NewPos.z - 1.0f;
                l_BMax.y = l_CurPos.z + 1.0f;
            }
            else {
                l_BMax.y = l_NewPos.z + 1.0f;
                l_BMin.y = l_CurPos.z - 1.0f;
            }

            l_SeadZone.Init(l_SeadZoneDatas);
            if (l_ObjectMove->IsFlagEnable(COL_BOL_LOD | COL_BOL_DECOR | COL_BOL_SPLINE | COL_BOL_VOLUME | COL_BOL_SHADOW | COL_BOL_DYNAMIC | COL_BOL_REFLECT | COL_BOL_UPDATE_GROUND_LIGHTING | COL_BOL_DEPTH | COL_BOL_COLLECTABLE | COL_BOL_UPDATE_CARRY_COLLISION | COL_BOL_SCAN_CLING | COL_BOL_LAND_AT_CLING)) {
                l_SeadZone.GetWorld()->GetCollisionZone(l_SeadZone, l_BMin, l_BMax, FL_NODE_COLLIDE, FL_NODE_HIDE);
            }
            l_ObjectMove->BeforeUpdateCollision(l_SeadZone, l_CurPos);
            l_ObjectMove->UpdateCollision(l_SeadZone, l_CurPos, l_NewPos, l_LookAt, l_Orient, i_DeltaTime, 0);
            l_ObjectMove->Update(l_SeadZone, l_CurPos, l_NewPos, l_LookAt, i_DeltaTime, 0);
            Node_Z* l_Node = l_ObjectMove->GetNode(0);
            l_Node->Update();
            l_ObjectMove->UpdateShadow(l_SeadZone, i_DeltaTime, 0);
            l_ObjectMove->RestoreCollision(l_SeadZone, 0);
        }
    }
}
