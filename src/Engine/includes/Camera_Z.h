#ifndef _CAMERA_Z_H_
#define _CAMERA_Z_H_
#include "Object_Z.h"
#include "Node_ZHdl.h"

struct FrustrumPlane_Z {
    Vec4f m_Planes[6];       // plane normals - left,right,bottom,top,near,far I think
    Vec4f m_PlanesDir[6];    // plane normals transformed by world matrix WITHOUT translation
    Float m_PlaneOffsets[6]; // the D (or -D im not sure) of the planes (you can move up the normal by this to get the real world coord plane)
    Mat4x4 m_PlanesLRB;      // L.x, R.x, B.x, 1.0, L.y, R.y, B.y, 1.0, L.z, R.z, B.z, 1.0, L.offset, R.offset, B.offset, 1.0
    Mat4x4 m_PlanesTNF;      // T.x, N.x, F.x, 1.0, T.y, N.y, F.y, 1.0, T.z, N.z, F.z, 1.0, T.offset, N.offset, F.offset, 1.0
    Vec4f m_PlaneX;          // x component of left,right,bottom,top
    Vec4f m_PlaneY;          // y component of left,right,bottom,top
    Vec4f m_PlaneZ;          // z component of left,right,bottom,top
};

struct Frustrum_Z {
    Mat4x4 m_WorldMatrix;
    Mat4x4 m_UnkMatrix;
    FrustrumPlane_Z m_Planes1;
    FrustrumPlane_Z m_Planes2;
    Float m_NearClip;
    Float m_OccludedFarClip;
    Float m_FarClip;
    Float m_UnkBytes_0x3ac;
    Float m_UnkBytes_0x3b0;
    Float m_UnkBytes_0x3b4;
    Float m_UnkBytes_0x3b8;
    Vec3f m_Direction;
    Vec3f m_UnkVec3f_0x3c8;
    Float m_UnkBytes_0x3d4;
    Float m_UnkBytes_0x3d8;
    Float m_UnkBytes_0x3dc;
    Float m_UnkBytes_0x3e0;
    Float m_UnkBytes_0x3e4;
    Float m_UnkBytes_0x3e8;
    Float m_UnkBytes_0x3ec;
    Float m_UnkBytes_0x3f0;
    Float m_UnkBytes_0x3f4;
    Float m_UnkBytes_0x3f8;
    Vec3f m_WorldTranslation;
    Float m_UnkBytes_0x408;
    Float m_UnkBytes_0x40c;
    Float m_FrustumFarClip;
    Vec3f m_CornerPoints[8]; // 0..3 are near, 4..7 are far
};

struct OccludedFrustum_Z {
    Vec2f m_TargetDelta;
    Vec2f m_Points[97];
    Float m_UnkFloats[97];               // obviously related to m_Points (lengths or something?)
    Vec2f m_DeltaPoints[96];             // difference between point at i and i+1
    Vec2f m_AccumulativeDeltaPoints[97]; // difference between point at i and point at 0
    Vec3f m_WorldPos;
    Vec3f m_Target;
    U32 m_PointNb;
    Vec2f m_SomeDiagonalMaybe1;
    Vec2f m_SomeDiagonalMaybe2;
    Bool m_IsOccluding;
};

class Camera_Z : public Object_Z {
private:
    Frustrum_Z m_Frustum;
    Mat4x4 m_WorldMatrix;
    Mat4x4 m_InverseWorldMatrix[2]; // Double buffer
    Float m_Fov;
    Float m_Roll;
    Vec3f m_Target;
    Vec3f m_WorldPos;
    Vec3f m_Direction;
    Node_ZHdl m_NodeTargetHdl;
    BaseObject_ZHdl m_OccluderHdl;
    BitArray_Z m_OccludedZonesBA; // possibly bit array of occluded zones
    OccludedFrustum_Z m_OccludedFrustum;
    Float m_DistFovEdition; // Name from Monopoly
    Float m_UnkFloat_0x10d0;
    Float m_UnkFloat_0x10d4;
    Float m_UnkFloat_0x10d8;
    Float m_UnkFloat_0x10dc;

public:
    void UpdateInverseWorldMatrix(Node_Z* i_Node);
    virtual ~Camera_Z();                                      /* 0x08 */
    virtual void Load(void** a1);                             /* 0x0C */
    virtual void EndLoad();                                   /* 0x10 */
    virtual void Draw(DrawInfo_Z& a1, ObjectDatas_Z* a2);     /* 0x14 */
    virtual void UpdateObject(Node_Z* a1, ObjectDatas_Z* a2); /* 0x18 */

    void SetFov(Float i_Fov);

    inline void SetRoll(Float i_Roll) {
        m_Roll = i_Roll;
    }

    void SetNearClip(Float i_Near);
    void SetFarClip(Float i_Far);
};

#endif
