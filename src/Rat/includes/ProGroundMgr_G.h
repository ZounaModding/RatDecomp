#ifndef _PROGROUNDMGR_G_H_
#define _PROGROUNDMGR_G_H_
#include "Creatures_G.h"
#include "Math_Z.h"
#include "ObjectGame_Z.h"

#define PRO_GROUND_GRID_SIZE 8

struct ProceduralObjectDef {
    const Char* m_Name;
    S32 m_Type;
    U32 m_Flags;
    S32 m_Count;
};

extern ProceduralObjectDef procedural[4];

struct GROUNDINFO {
    GROUNDINFO() { m_ObjectHdl = HANDLE_NULL; }

    Vec3f m_Pos;
    Vec3f m_Normal;
    BaseObject_ZHdl m_ObjectHdl;
    U32 m_Flags;
    Bool m_HasGround;
    Bool m_Valid;
    U8 m_Pad_0x22[2];
};

class ProGroundMgr_G : public ObjectGame_Z {
public:
    static BaseObject_Z* NewObject() { return NewL_Z(40) ProGroundMgr_G; }

    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    void SetGame(const Game_ZHdl& i_GameHdl);
    void AddLinkedObjectStack(const Name_Z& i_Name, S32 i_Type);

private:
    ObjectLinked_ZDA m_LinkedObjects;
    U8 m_Unk_0x34[20];
    GROUNDINFO m_GroundInfos[PRO_GROUND_GRID_SIZE][PRO_GROUND_GRID_SIZE];
};
#endif // _PROGROUNDMGR_G_H_
