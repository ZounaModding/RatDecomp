#ifndef _CREATURES_G_H_
#define _CREATURES_G_H_
#include "BaseAgent_G.h"
#include "CreaturesBoneManipHdl.h"
#include "DynArray_Z.h"
#include "DynPtrArray_Z.h"
#include "Math_Z.h"

class CreaturesMove_G;
class InGameCreatures_G;
class Node_Z;

struct ObjectLinked_Z {
    U8 m_Unk_0x00[0x50];
};

typedef DynArray_Z<ObjectLinked_Z, 8> ObjectLinked_ZDA;
// clang-format off

BEGIN_AGENT_CLASS(Creatures_G, BaseAgent_G, 34)
public:
    Creatures_G();

    virtual ~Creatures_G() { RemoveFromStaticList(); }
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(ParticlesEfx);
    DECL_BHV(ParticlesEfxPause);
    DECL_BHV(ParticlesEfxReset);
    DECL_BHV(AnimChange);
    DECL_BHV(Contact);
    DECL_BHV(NoteTrackSample);
    DECL_BHV(CheckRtcScriptMessage);
    DECL_BHV(EnterTeleportZone);
    DECL_BHV(ExecuteTeleportZone);
    DECL_BHV(FX_WeaponBlur);
    DECL_BHV(CheckFootOnGround);
    DECL_BHV(CheckFootPrintOnGround);
    DECL_BHV(CheckStartMission);

private:
    static DynPtrArray_Z<Creatures_G*> instances;
    Float m_LinkedObjectCycleTimer;
    Float m_UnkLinkedRelated_0x80;
    Bool m_UnkBool_0x84;
    ObjectLinked_ZDA m_LinkedObjectArray;
    InGameCreatures_G* m_InGameCreatures;
    CreaturesMove_G* m_CreaturesMove;
    Node_Z* m_CreatureNode;
    S32 m_BaseParamValue;
    Float m_ImpactRelated;
    Float m_Unk_0xa4;
    S64 m_Unk_0xa8;
    Quat m_CreatureRot;
    Vec3f m_CreaturePos;
    Bool m_TeleportingOrSomething;
    S32 m_Unk_0xd0;
    Bool m_Hidden;
    Bool m_UnkBool_0xd5;
    CreaturesBoneManipHdl m_CreaturesBoneManipHdl;
    U32 m_NbImpacts;
    Float m_UnkFloat_0xe0;
    Float m_UnkFloat_0xe4;
    S32 m_Unk_0xe8;
    S32 m_ActionContextId;
END_AGENT_CLASS

// clang-format on
#endif // _CREATURES_G_H_
