#ifndef _OBJECT_Z_H_
#define _OBJECT_Z_H_
#include "ResourceObject_Z.h"
#include "SystemObject_Z.h"
#include "Collision_Z.h"
#include "Object_ZHdl.h"
#include "StaticArray_Z.h"

class Node_Z;

enum ObjectType {
    UNDEFINED_GEOMETRY = 0,
    SURFACE_Z = 1,
    SPLINE_Z = 2,
    SKIN_Z = 3,
    ROTSHAPE_Z = 4,
    LOD_Z = 5,
    MESH_Z = 6,
    CAMERA_Z = 7,
    SPLINEZONE_Z = 9,
    OCCLUDER_Z = 10,
    CAMERAZONE_Z = 11,
    LIGHT_Z = 12,
    HFOG_Z = 13,
    COLLISIONVOL_Z = 14,
    EMITER_Z = 15,
    OMNI_Z = 16,
    GRAPH_Z = 17,
    PARTICLES_Z = 18,
    FLARE_Z = 19,
    HFIELD_Z = 20,
    TREE_Z = 21,
};

// ObjectDatas_Z flags

#define FL_OBJECTDATAS_NONE (0 << 0)
#define FL_OBJECTDATAS_HIDE (1 << 1)                                    // 0x2 - Object data is hidden
#define FL_OBJECTDATAS_CLONED (1 << 3)                                  // 0x8 - Object data is cloned
#define FL_OBJECTDATAS_SKINNED (1 << 4)                                 // 0x10 - Skinned geometry
#define FL_OBJECTDATAS_VREFLECT (1 << 5)                                // 0x20 - Fake vertical mirror to simulate reflection on the ground
#define FL_OBJECTDATAS_HREFLECT (1 << 6)                                // 0x40 - Fake vertical mirror to simulate reflection on the wall
#define FL_OBJECTDATAS_STATIC_SHADOW (1 << 7)                           // 0x80 - Deprecated
#define FL_OBJECTDATAS_DISPLAY (1 << 8)                                 // 0x100 - Display the object data
#define FL_OBJECTDATAS_NO_FOG (1 << 9)                                  // 0x200 - Don't use fog for this object data
#define FL_OBJECTDATAS_LIGHTING_UPDATED (1 << 10)                       // 0x400 - Lighting is up to date
#define FL_OBJECTDATAS_UPDATE_BSPHERE (1 << 11)                         // 0x800 - BSphere needs to be updated
#define FL_OBJECTDATAS_VP0_HIDE (1 << 12)                               // 0x1000 - Hide object data for viewport 0
#define FL_OBJECTDATAS_VP1_HIDE (1 << 13)                               // 0x2000 - Hide object data for viewport 1
#define FL_OBJECTDATAS_VP2_HIDE (1 << 14)                               // 0x4000 - Hide object data for viewport 2
#define FL_OBJECTDATAS_VP3_HIDE (1 << 15)                               // 0x8000 - Hide object data for viewport
#define FL_OBJECTDATAS_VPX_HIDE(x) (FL_OBJECTDATAS_VP0_HIDE << (U8)(x)) // 0x1000/0x2000/0x4000/0x8000 - Hide object data for viewport 1/2/33
#define FL_OBJECTDATAS_LAST (FL_OBJECTDATAS_VP3_HIDE << 1)              // 0x10000 - Last flag, user flags begin from here

// Object_Z flags

#define FL_OBJECT_NONE (0 << 0)
#define FL_OBJECT_MAX_BSPHERE (1 << 0)             // 0x1 - BSphere is already at max radius, so it shouldn't be recalculated anymore
#define FL_OBJECT_SKINNED (1 << 1)                 // 0x2 - Skinned geometry
#define FL_OBJECT_NO_SEADDISPLAY (1 << 2)          // 0x4 - Don't use SEADS for display
#define FL_OBJECT_NO_SEADCOLLIDE (1 << 3)          // 0x8 - Don't use SEADS for collide
#define FL_OBJECT_UNK_0x10 (1 << 4)                // 0x10 - Unknown (Used in Mesh_Z)
#define FL_OBJECT_DISPLAY (1 << 5)                 // 0x20 - Display the object
#define FL_OBJECT_NO_DISPLAY (1 << 6)              // 0x40 - Don't display the object
#define FL_OBJECT_IS_SHADOW_VOLUME (1 << 7)        // 0x80 - Shadow volume mesh
#define FL_OBJECT_HAS_SHADOW_VOLUME (1 << 8)       // 0x100 - Set on meshes that have a shadow mesh attached (to stop it from drawing on top of the visual mesh)
#define FL_OBJECT_OMNI_REJECT_CHANNEL_0 (1 << 9)   // 0x200 - Reject lighting by Omni lights in channel 0
#define FL_OBJECT_OMNI_REJECT_CHANNEL_1 (1 << 10)  // 0x400 - Reject lighting by Omni lights in channel 1
#define FL_OBJECT_OMNI_REJECT_CHANNEL_2 (1 << 11)  // 0x800 - Reject lighting by Omni lights in channel 2
#define FL_OBJECT_OMNI_REJECT_CHANNEL_3 (1 << 12)  // 0x1000 - Reject lighting by Omni lights in channel 3
#define FL_OBJECT_OMNI_REJECT_CHANNEL_4 (1 << 13)  // 0x2000 - Reject lighting by Omni lights in channel 4
#define FL_OBJECT_OMNI_REJECT_CHANNEL_5 (1 << 14)  // 0x4000 - Reject lighting by Omni lights in channel 5
#define FL_OBJECT_OMNI_REJECT_CHANNEL_6 (1 << 15)  // 0x8000 - Reject lighting by Omni lights in channel 6
#define FL_OBJECT_OMNI_REJECT_CHANNEL_7 (1 << 16)  // 0x10000 - Reject lighting by Omni lights in channel 7
#define FL_OBJECT_OMNI_REJECT_CHANNEL_8 (1 << 17)  // 0x20000 - Reject lighting by Omni lights in channel 8
#define FL_OBJECT_OMNI_REJECT_CHANNEL_9 (1 << 18)  // 0x40000 - Reject lighting by Omni lights in channel 9
#define FL_OBJECT_OMNI_REJECT_CHANNEL_10 (1 << 19) // 0x80000 - Reject lighting by Omni lights in channel 10
#define FL_OBJECT_OMNI_REJECT_CHANNEL_X(x) (FL_OBJECT_OMNI_REJECT_CHANNEL_0 << (x))

#define FL_OBJECT_OMNI_REJECT_ALL (FL_OBJECT_OMNI_REJECT_CHANNEL_0 | FL_OBJECT_OMNI_REJECT_CHANNEL_1 | FL_OBJECT_OMNI_REJECT_CHANNEL_2 | FL_OBJECT_OMNI_REJECT_CHANNEL_3 | FL_OBJECT_OMNI_REJECT_CHANNEL_4 | FL_OBJECT_OMNI_REJECT_CHANNEL_5 | FL_OBJECT_OMNI_REJECT_CHANNEL_6 | FL_OBJECT_OMNI_REJECT_CHANNEL_7 | FL_OBJECT_OMNI_REJECT_CHANNEL_8 | FL_OBJECT_OMNI_REJECT_CHANNEL_9 | FL_OBJECT_OMNI_REJECT_CHANNEL_10) // 0xFFE00 - Reject lighting by Omni lights in all channels

// TODO: Clean up below (move to own files). These are user flags, specified by the classes that inherit from Object_Z, starting from FL_OBJECT_LAST (1 << 20)
#define FL_OBJECT_LAST (1 << 20)

// Lod_Z flags

#define FL_IS_LOD_SKIN (FL_OBJECT_LAST << 0) // 0x100000 - LOD is a skin

// Spline_Z flags

#define FL_IS_SPLINE_TRACK (FL_OBJECT_LAST << 0)      // 0x100000 - Spline is a track
#define FL_IS_SPLINE_TRACK_LOOP (FL_OBJECT_LAST << 1) // 0x200000 - Spline track is a loop
#define FL_IS_SPLINE_DONOTOUCH (FL_OBJECT_LAST << 2)  // 0x400000 - Don't let the engine clean the spline

// Light_Z and Omni_Z shared flags (In case of Light_Z they're actually stored in LightData_Z::m_LightFlag)

#define FL_IS_LIGHT_ACTIVE (FL_OBJECT_LAST << 0)        // 0x100000 - Light is active
#define FL_IS_LIGHT_UNK_0x200000 (FL_OBJECT_LAST << 1)  // 0x200000 - Unknown
#define FL_IS_LIGHT_UNK_0x400000 (FL_OBJECT_LAST << 2)  // 0x400000 - Unknown
#define FL_IS_LIGHT_DYNAMIC (FL_OBJECT_LAST << 3)       // 0x800000 -  Can light objects at runtime
#define FL_IS_LIGHT_UNK_0x1000000 (FL_OBJECT_LAST << 4) // 0x1000000 - Unknown

// Light_Z flags (Are actually stored in LightData_Z::m_LightFlag))

#define FL_IS_LIGHT_POSITIONAL (FL_OBJECT_LAST << 5) // 0x2000000 - Semi omni light approach for actors (LOD), each one will have its own direction as if it were a point light

// Omni_Z flags

#define FL_IS_OMNI_PRIORITY_MID (FL_OBJECT_LAST << 5)  // 0x2000000  - 2nd-priority omni bucket in SetOmnis 3-slot light sort (when there's more than 3 omnis to choose from)
#define FL_IS_OMNI_UNK_0x4000000 (FL_OBJECT_LAST << 6) // 0x4000000  - Unknown
#define FL_IS_OMNI_SPOTLIGHT (FL_OBJECT_LAST << 7)     // 0x8000000  - Omni_Z is a spotlight
#define FL_IS_OMNI_PRIORITY_HIGH (FL_OBJECT_LAST << 8) // 0x10000000 - 1st-priority omni bucket in SetOmnis 3-slot light sort (when there's more than 3 omnis to choose from)

class ObjectDatas_Z : public ResourceObject_Z {
public:
    ObjectDatas_Z();

    virtual ~ObjectDatas_Z() { }

    virtual void Load(void** i_Data);
    virtual Bool MarkHandles();
    virtual void UpdatePointers(Object_Z* i_Obj, S32 i_Param = -1);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void HideObject(S32 i_Index);
    virtual void UnHideObject(S32 i_Index);
    virtual void SetShadowData(Node_Z* i_Node, const Vec3f& i_Origin, const ColLineResult_Z& i_Result, Float i_Radius);
    virtual void UpdateShadowData(Node_Z* i_Node, const Vec3f& i_Origin, Float i_Radius);
    virtual void UpdateReflectData(Node_Z* i_Node, const Vec3f& i_Origin);

    virtual void SetDfltColor(const Color& i_Color) {
        m_DefaultColor = i_Color;
    }

    virtual void SetDfltColorNoAlpha(const Color& i_Color);
    virtual void SetDfltColorAlpha(Float i_Alpha);
    virtual void FreeLightCacheEntry(U16 i_Index);

protected:
    U32 m_Flag;
    Color m_DefaultColor;
};

class Object_Z : public ResourceObject_Z {
public:
    Object_Z();

    // $SABE TODO: Vtable not getting generated atm?
    virtual void Init() { }

    virtual ~Object_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void LoadLinks(void** i_Data);
    virtual void EndLoadLinks();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    virtual void DrawReceiveShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data, S32 i_ObjID) { }

    virtual void DrawCastingShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data);

    virtual Bool GetCollisionVLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Vec4f& i_Vec, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual Bool GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual Bool GetClingLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ClingLineResult_Z& o_Result) {
        return FALSE;
    }

    virtual Bool GetCollisionSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Seg, StaticArray_Z<ColSphereResult_Z, 256, 0, 1>& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual Bool GetCollisionMovingSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual Bool GetCollisionCapsule(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, StaticArray_Z<ColSphereResult_Z, 128, 0, 1>& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual Bool GetCollisionBoxes(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<BoxFlag_Z, 8, FALSE, FALSE, 4>& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual Bool GetCollisionTriangles(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<TriangleFlag_Z, 8, FALSE, FALSE, 4>& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual Bool GetCollisionSpheres(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<SphereFlag_Z, 8, FALSE, FALSE, 4>& o_Result, U64 i_Flag, U64 i_NoFlag) {
        return FALSE;
    }

    virtual S32 GetCollisions(
        Node_Z* i_Node,
        ObjectDatas_Z* i_Data,
        const Sphere_Z& i_Sph,
        const Vec3f& i_Dir,
        Float i_Len,
        ColLineResult_Z* o_Result,
        S32 i_MaxElem,
        U64 i_Flag,
        U64 i_NoFlag
    ) {
        return 0;
    }

    inline U16 GetGeometryType() const {
        return m_Type;
    }

    inline void SetGeometryType(U16 i_Type) {
        m_Type = i_Type;
    }

    inline U32 GetFlag() const {
        return m_Flag;
    }

    inline void SetFlag(const U32 i_Flag) {
        m_Flag = i_Flag;
    }

    inline Bool IsFlag(const U32 i_Flag) const {
        return (m_Flag & i_Flag) ? TRUE : FALSE;
    }

    inline void EnableFlag(const U32 i_Flag) {
        m_Flag |= i_Flag;
    }

    inline void DisableFlag(const U32 i_Flag) {
        m_Flag &= (U32)(~i_Flag);
    }

    inline const Sphere_Z& GetBSphere() const {
        return m_BSphereLocal;
    }

    inline void SetBSphere(Sphere_Z& i_Sph) {
        m_BSphereLocal = i_Sph;
    }

    inline const Box_Z& GetBBox() const {
        return m_BBoxLocal;
    }

    inline void SetBBox(Box_Z& i_Box) {
        m_BBoxLocal = i_Box;
    }

    inline Float GetFadeDist() const {
        return m_FadeOutDistance;
    }

protected:
    Sphere_Z m_BSphereLocal;
    Box_Z m_BBoxLocal;
    Float m_FadeOutDistance;
    U32 m_Flag;
    U16 m_Type;
    ObjectDatas_ZHdl m_ObjectDataHdl;
};

#endif
