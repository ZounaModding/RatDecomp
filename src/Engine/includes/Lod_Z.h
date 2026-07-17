#ifndef _LOD_Z_H_
#define _LOD_Z_H_
#include "DynArray_Z.h"
#include "Math_Z.h"
#include "LightData_Z.h"
#include "Node_ZHdl.h"
#include "Object_ZHdl.h"
#include "Light_ZHdl.h"

#define FL_LOD_FORCE_HIGHEST (FL_OBJECTDATAS_LAST)               // 0x10000 - Force highest LOD
#define FL_LOD_DISABLE_STENCIL_SHADOW (FL_OBJECTDATAS_LAST << 1) // 0x20000 - Disable stencil shadow for this LOD

struct ShadowReceiver_Z {
    Node_ZHdl m_NodeHdl;                  // node we collided with hdl
    Object_ZHdl m_ObjectHdl;              // object we collided with hdl
    ObjectDatas_ZHdl m_ObjectDataHdl;     // object data we collided with hdl
    S32DA m_ShadowReceiverElementIndices; // indices of the elements of the object that receive shadow (ex: for surface, the patch ids)
};

typedef DynArray_Z<ShadowReceiver_Z, 2, 1, 1, 4> ShadowReceiver_ZDA;

struct ActorData_Z {
    Vec3f m_LocalShadowPos;
    Float m_ShadowRadius;
    Vec3f m_WorldShadowPos;
    Float m_LightInterpolationTime; // used to switch light
    Vec3f m_WorldShadowNormal;
    U32 m_Flag;
    Vec3f m_LocalShadowBoundsCenter;
    Float m_LocalShadowBoundsRadius;
    LightData_Z m_LightData;
    LightData_ZHdl m_CurLightDataHdl;
    ShadowReceiver_ZDA m_ShadowReceivers;
    Vec4f m_UnkVec4f_0xbc;
    Bool m_UnkBool_0xcc;
};

class LodData_Z : public ObjectDatas_Z {
public:
    LodData_Z();
    virtual ~LodData_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual Bool MarkHandles();
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void SetShadowData(Node_Z* i_Node, const Vec3f& i_Origin, const ColLineResult_Z& i_Result, Float i_Radius);
    virtual void UpdateShadowData(Node_Z* i_Node, const Vec3f& i_Origin, Float i_Radius);
    virtual void UpdateReflectData(Node_Z* i_Node, const Vec3f& i_Origin);
    virtual void SetDfltColor(const Color& i_Color);
    virtual void SetDfltColorNoAlpha(const Color& i_Color);
    virtual void SetDfltColorAlpha(Float i_Alpha);

protected:
    U8 m_Pad_0x28[16];
};

class Lod_Z : public Object_Z {
public:
    Lod_Z();
    virtual ~Lod_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual Bool GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionMovingSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
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
    );

protected:
    U8 m_Pad_0x80[64];
};

#endif // _LOD_Z_H_
