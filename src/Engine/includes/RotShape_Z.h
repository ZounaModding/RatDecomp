#ifndef _ROTSHAPE_Z_H_
#define _ROTSHAPE_Z_H_
#include "DynArray_Z.h"
#include "Handle_Z.h"
#include "Points_Z.h"
#include "Points_ZHdl.h"
#include "Material_ZHdl.h"

#define ROTSHAPE_TYPE_VERTICAL 0    // Only rotate around Y axis
#define ROTSHAPE_TYPE_ALWAYSFRONT 1 // Always face the camera in all axes

#define FL_ROTSHAPE_HIDE (1 << 3)   // RotShapeData is hidden
#define FL_ROTSHAPE_SCALED (1 << 4) // RotShapeData is scaled
#define FL_ROTSHAPE_SHADOW (1 << 5) // RotShapeData casts shadows

class RotShapeData_Z;
class RotShape_Z;

// $SABE: Figure this out if needed
struct RotShapeData {
    Float m_Unk_0x0;
    Float m_Unk_0x4;
    Float m_Unk_0x8;
    Float m_Unk_0xc;
    Float m_Unk_0x10;
    Float m_Unk_0x14;
    Float m_Unk_0x18;
};

typedef DynArray_Z<RotShapeData, 32, FALSE, FALSE> RotShapeDataDA;

HANDLE_Z(RotShapeData_Z, PointsDatas_Z);
HANDLE_Z(RotShape_Z, Points_Z);
typedef DynArray_Z<RotShape_ZHdl, 32> RotShape_ZHdlDA;

typedef DynArray_Z<RotShape_Z, 32> RotShape_ZDA;

class RotShape_Z : public Points_Z {
public:
    RotShape_Z();

    virtual ~RotShape_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
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
    S32DA m_MatIndices;
    Vec3fDA m_LocalVertices;
    Vec2fDA m_LocalUVs;
    Material_ZHdlDA m_MaterialHdls;
    Float m_Scale;
    U16 m_Type;
};

class RotShapeData_Z : public PointsDatas_Z {
public:
    RotShapeData_Z();
    virtual ~RotShapeData_Z();
    virtual void Load(void** i_Data);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual void HideObject(S32 i_Index = -1);
    virtual void UnHideObject(S32 i_Index = -1);
    virtual void SetShadowData(Node_Z* i_Node, const Vec3f& i_Origin, const ColLineResult_Z& i_Result, Float i_Radius);

protected:
    U16DA m_FlagsDA;
    RotShapeDataDA m_RotShapeDatas;
};
#endif
