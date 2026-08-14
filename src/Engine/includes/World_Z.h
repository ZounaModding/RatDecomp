#ifndef _WORLD_Z_H_
#define _WORLD_Z_H_
#include "ResourceObject_Z.h"
#include "Node_ZHdl.h"
#include "OccludedSeadHandle_Z.h"
#include "AnimFrame_ZHdl.h"
#include "Graph_ZHdl.h"
#include "ManipulatorDraw_ZHdl.h"
#include "CameraZone_ZHdl.h"
#include "Occluder_ZHdl.h"
#include "StaticArray_Z.h"
#include "HoleArray_Z.h"
#include "World_ZHdl.h"
#include "HField_ZHdl.h"
#include "Warp_ZHdl.h"
#include "HFog_ZHdl.h"
#include "GenWorld_ZHdl.h"
#include "GameObj_ZHdl.h"
#include "Flare_ZHdl.h"

#define MAX_CAMERAZONE_OBJECT 8
#define MAX_OCCLUDER_OBJECT 8
#define MAX_FLARE_OBJECT 8

struct SubWorldData_Z {
    U8 m_Pad_0x0[0x168];
};

typedef DynArray_Z<SubWorldData_Z, 8> SubWorldData_ZDA;

class World_Z : public ResourceObject_Z {
public:
    virtual ~World_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void LoadDone();
    virtual Bool MarkHandles();

    void Draw(DrawInfo_Z& i_DrawInfo);
    void EndDraw();

    Node_ZHdl GetNodeByName(const Name_Z& i_Name) const;
    void Update(Float i_DeltaTime);
    void Assume();

    inline OccludedSeadHandle_Z& GetSeadDisplay() {
        return m_SeadDisplay;
    }

    inline SeadHandle_Z& GetSeadCollide() {
        return m_SeadCollide;
    }

    inline Node_ZHdl& GetRoot() {
        return m_RootNodeHdl;
    }

    inline HField_ZHdl GetHField() {
        return m_HFieldHdl;
    }

    inline S32 GetNbOccluder() {
        return m_OccluderHdls.GetSize();
    }

    inline Occluder_ZHdl& GetOccluder(S32 i_Idx) {
        return m_OccluderHdls[i_Idx];
    }

    inline S32 GetNbVp() const {
        return m_NbVp;
    }

    inline S32 GetFirstVp() const {
        return m_FirstPlayerVpId;
    }

    inline S32 GetNbManipulatorSceneDraw() const {
        return m_ManipulatorSceneDrawHdls.GetSize();
    }

    inline const ManipulatorSceneDraw_ZHdl& GetManipulatorSceneDraw(S32 i_Idx) const {
        return m_ManipulatorSceneDrawHdls[i_Idx];
    }

    static BaseObject_Z* NewObject() { return NewL_Z(314) World_Z; }

private:
    U32 m_Flag;
    S32 m_NbVp;
    S32 m_FirstPlayerVpId;
    Warp_ZHdl m_WarpHdl;
    HField_ZHdl m_HFieldHdl;
    GenWorld_ZHdl m_GenWorldHdl;
    GameObj_ZHdl m_GameObjHdl;
    Node_ZHdl m_RootNodeHdl;
    HFogData_ZHdl m_MainHFogDataHdl;
    OccludedSeadHandle_Z m_SeadDisplay;
    SeadHandle_Z m_SeadCollide;
    AnimFrame_ZHdlDA m_AnimFrameHdls;
    Graph_ZHdlDA m_GraphHdls;
    Node_ZHdlDA m_NoSeadNodeHdls; // Not sure
    Node_ZHdlDA m_NoFrustumClippingNodeHdls;
    Node_ZHdlDA m_NoOccluderClippingNodeHdls;
    SubWorldData_ZDA m_SubWorldDatas;
    ManipulatorSceneDraw_ZHdlDA m_ManipulatorSceneDrawHdls;
    StaticArray_Z<CameraZone_ZHdl, MAX_CAMERAZONE_OBJECT> m_CameraZoneHdls;
    StaticArray_Z<Occluder_ZHdl, MAX_OCCLUDER_OBJECT> m_OccluderHdls;
    // $SABE: We don't actually know which handle type this stores. The constructor hierarchy lines up with a child of Object_Z,
    //        and Flare_Z was introduced in the same game as this static array (The Mummy), since it works out I decided to use it.
    //        Could also be a static array of FlareData_ZHdl, which would line up with how they store fog above (HFogData_ZHdl).
    //        Note that this goes completely unused in all games that have it, aside from attempting to load and mark the handles.
    //        Maybe they first assumed they could store flare objects at the world level, but later decided to move them into nodes.
    StaticArray_Z<Flare_ZHdl, MAX_FLARE_OBJECT> m_FlareHdls;
    // TODO: More members
};

class WorldManager_Z {
protected:
    HoleArray_Z<World_ZHdl, 8> m_WorldHdls;

public:
    WorldManager_Z();
    virtual ~WorldManager_Z();

    inline void MarkHandles() {
        S32 i = m_WorldHdls.FindFirst();
        while (i >= 0) {
            m_WorldHdls[i]->MarkHandles();
            i = m_WorldHdls.FindNext(i);
        }
    }

    inline void Assume() {
        S32 i = m_WorldHdls.FindFirst();
        while (i >= 0) {
            m_WorldHdls[i]->Assume();
            i = m_WorldHdls.FindNext(i);
        }
    }

    inline const World_ZHdl& GetWorld(S32 i_Index) const {
        return m_WorldHdls.Get(i_Index);
    }
};

#endif // _WORLD_Z_H_
