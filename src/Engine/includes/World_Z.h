#ifndef _WORLD_Z_H_
#define _WORLD_Z_H_
#include "ResourceObject_Z.h"
#include "Node_ZHdl.h"
#include "OccludedSeadHandle_Z.h"
#include "AnimFrame_ZHdl.h"
#include "Graph_ZHdl.h"
#include "ManipulatorDraw_ZHdl.h"
#include "CameraZone_ZHdl.h"
#include "CameraZoneData_Z.h"
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
#include "GameManager_Z.h"
#include "Camera_Z.h"

#define MAX_CAMERAZONE_OBJECT 8
#define MAX_OCCLUDER_OBJECT 8
#define MAX_FLARE_OBJECT 8

#define FL_WORLD_NONE (0 << 0)
#define FL_WORLD_DISABLE_SCENE_DRAW (1 << 0) // 0x1 - Disable world draw

enum SceneOrder {
    so_none = 0,
    so_scene = 1,
    so_2d = 2,
    so_last = 3,
    so_count = 4
};

struct Line2D_Z {
    Vec3f m_P1;
    Vec3f m_P2;
    Color m_Col;
};

struct Face3D_Z {
    Vec3f m_P1;
    Vec3f m_P2;
    Vec3f m_P3;
    Color m_Col;
};

struct PosName_Z {
    Vec3f m_Pos;
    Color m_Col;
    String_Z<ARRAY_CHAR_MAX> m_Str;
};

struct PosName2D_Z {
    Vec2f m_Pos;
    Color m_Col;
    String_Z<ARRAY_CHAR_MAX> m_Str;
};

typedef DynArray_Z<Line2D_Z, 32, FALSE, FALSE> Line2D_ZDA;
typedef DynArray_Z<Face3D_Z, 32, FALSE, FALSE> Face3D_ZDA;
typedef DynArray_Z<PosName_Z, 32> PosName_ZDA;
typedef DynArray_Z<PosName2D_Z, 32> PosName2D_ZDA;

struct SubWorldRange_Z {
    Vec2fRect_Z m_Rect;
    Float m_Unk_0x10;
    Float m_Unk_0x14;
    Vec2fDA m_PolyVertices; // Points that form a polygon
    Box_ZDA m_Boxes;        // Bounding boxes?
};

enum SubWorldType {
    SUBWORLD_TYPE_NONE = 0,
    SUBWORLD_TYPE_SUBLEVEL = 1,
    SUBWORLD_TYPE_SUBDATA = 2
};

#define FL_SUBWORLD_NONE (0 << 0)
#define FL_SUBWORLD_LOADING (1 << 0)
#define FL_SUBWORLD_LOADED (1 << 1)
#define FL_SUBWORLD_REMOVING (1 << 2)

struct SubWorldData_Z {
    S32 m_Type; // 1 == sublevel, 2 == subdata?
    S32 m_Unk_0x4;
    S32 m_SubId;
    S32 m_Unk_0xc;
    S32 m_Unk_0x10;
    S32 m_Unk_0x14;
    SubWorldRange_Z m_Range;
    Char m_SubWorldName[256];
    S32DA m_UnkIds_0x140;
    S32DA m_UnkIds_0x148;
    S32DA m_SubIds;
    U8 m_Flag;
    SubWorld_ZHdl m_SubWorldHdl;
    S32 m_Unk_0x160;
    S32 m_Unk_0x164;
};

typedef DynArray_Z<SubWorldData_Z, 8> SubWorldData_ZDA;

class World_Z : public ResourceObject_Z {
public:
    World_Z() {
        m_NbVp = 0;
        m_FirstPlayerVpId = -1;
        m_Flag = FL_WORLD_NONE;
    }

    virtual ~World_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void LoadDone();
    virtual Bool MarkHandles();

    void Draw(DrawInfo_Z& i_DrawInfo);
    void DrawObjects(DrawInfo_Z& i_DrawInfo);
    void GetOmniOnClip(DrawInfo_Z& i_DrawInfo, StaticArray_Z<OmniFrust_Z, 64, FALSE, FALSE>& io_OmniFrusts);
    void AddManipulatorSceneDraw(const ManipulatorSceneDraw_ZHdl& i_ManipulatorSceneDrawHdl);
    void EndDraw();

    Node_ZHdl GetNodeByName(const Name_Z& i_Name) const;
    S32 GetNodesByObjectType(Node_ZHdlDA& o_Nodes, S32 i_Type) const;
    Bool GetCameraZone(const Vec3f& i_WorldPosition, CameraZoneData_Z& o_Data, S32 i_CameraZoneId = -1);
    void NoFrustrumClipping(const Node_ZHdl& i_NodeHdl, Bool i_NoClipping);
    void Update(Float i_DeltaTime);
    void UpdateAnims(Float i_DeltaTime);
    void UpdateLighting(Bool i_Force);
    void RefWorldId(S16 i_WorldId);
    void ActionOnActivate();
    void Assume();
    void GetCollisionZone(SeadZone_Z& io_Zone, const Vec2f& i_BMin, const Vec2f& i_BMax, U32 i_Flag, U32 i_NoFlag);

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

    inline Bool IsFlag(U32 i_Flag) const {
        return (m_Flag & i_Flag) ? TRUE : FALSE;
    }

    inline void EnableFlag(U32 i_Flag) {
        m_Flag |= i_Flag;
    }

    inline Warp_ZHdl GetWarp() {
        return m_WarpHdl;
    }

    inline GenWorld_ZHdl GetGenWorld() {
        return m_GenWorldHdl;
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
    // Debug draw data
    SeadZoneData_ZDA m_DebugLockViewSeadZoneDatas[MAX_VIEWPORT];
    BitArray_Z m_DebugLockViewOccludedZonesBAs[MAX_VIEWPORT];
    Camera_Z m_DebugLockViewCameras[MAX_VIEWPORT];
    Line2D_ZDA m_DebugLines2D;
    Face3D_ZDA m_DebugFaces3D;
    PosName_ZDA m_DebugNames;
    PosName2D_ZDA m_DebugNames2D;
};

class WorldManager_Z {
protected:
    HoleArray_Z<World_ZHdl, 8> m_WorldHdls;

public:
    WorldManager_Z();
    virtual ~WorldManager_Z();
    void AddWorld(World_ZHdl i_WorldHdl);

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
