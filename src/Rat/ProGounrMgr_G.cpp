#include "ProGroundMgr_G.h"

ProceduralObjectDef procedural[4] = {
    { "PROCEDURAL_1", 0x04000000, 0x0c000000, 128 },
    { "PROCEDURAL_2", 0x08000000, 0x0c000000, 128 },
    { "PROCEDURAL_3", 0x0c000000, 0x0c000000, 128 },
    { NULL, 0, 0, 0 },
};

void ProGroundMgr_G::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_game_manager);
    m_NbVp = 0;
    m_FirstPlayerVpId = -1;
    SetGroup(ag_game_manager);

    for (S32 i = 0; i < PRO_GROUND_GRID_SIZE; ++i) {
        for (S32 j = 0; j < PRO_GROUND_GRID_SIZE; ++j) {
            m_GroundInfos[i][j].m_Pos.x = -100000.f;
            m_GroundInfos[i][j].m_Pos.z = -100000.f;
            m_GroundInfos[i][j].m_HasGround = FALSE;
            m_GroundInfos[i][j].m_Valid = FALSE;
        }
    }
}

void ProGroundMgr_G::Reset() {
}

void ProGroundMgr_G::SetGame(const Game_ZHdl& i_GameHdl) {
    m_GameHdl = i_GameHdl;
    for (S32 i = 0; procedural[i].m_Name; ++i) {
        for (S32 j = 0; j < procedural[i].m_Count; ++j) {
            AddLinkedObjectStack(Name_Z(procedural[i].m_Name), procedural[i].m_Type);
        }
    }
}

void ProGroundMgr_G::Update(Float i_DeltaTime) {
}
