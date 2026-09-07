#include "ProGounrMgr_G.h"

struct ProceduralObjectDef {
    const Char* m_Name;
    S32 m_Type;
    void* m_Unk;
    S32 m_Count;
};

extern ProceduralObjectDef procedural[];

void ProGroundMgr_G::SetGame(const Game_ZHdl& i_GameHdl) {
    m_GameHdl = i_GameHdl;
    for (S32 i = 0; procedural[i].m_Name; ++i) {
        for (S32 j = 0; j < procedural[i].m_Count; ++j) {
            AddLinkedObjectStack(Name_Z(procedural[i].m_Name), procedural[i].m_Type);
        }
    }
}
