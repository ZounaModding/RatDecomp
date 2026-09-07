#ifndef _PROGOUNRMGR_G_H_
#define _PROGOUNRMGR_G_H_
#include "ObjectGame_Z.h"

struct ProceduralObjectDef {
    const Char* m_Name;
    S32 m_Type;
    U32 m_Flags;
    S32 m_Count;
};

extern ProceduralObjectDef procedural[4];

class ProGroundMgr_G : public ObjectGame_Z {
public:
    void SetGame(const Game_ZHdl& i_GameHdl);
    void AddLinkedObjectStack(const Name_Z& i_Name, S32 i_Type);

private:
    U8 m_Unk_0x2c[0x91c];
};
#endif // _PROGOUNRMGR_G_H_
