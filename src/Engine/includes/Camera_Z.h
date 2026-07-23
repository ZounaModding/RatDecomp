#ifndef _CAMERA_Z_H_
#define _CAMERA_Z_H_
#include "Object_Z.h"
#include "Node_ZHdl.h"
#include "MatrixCache_Z.h"
#include "Occluder_ZHdl.h"

struct FrustrumPlane_Z {
    Vec4f m_PlaneNormals[6]; // plane normals - left,right,bottom,top,near,far I think
    Vec4f m_PlanesDir[6];    // plane normals transformed by world matrix WITHOUT translation
    Float m_PlaneOffsets[6]; // the D (or -D im not sure) of the planes (you can move up the normal by this to get the real world coord plane)
    Mat4x4 m_PlanesLRB;      // L.x, R.x, B.x, 1.0, L.y, R.y, B.y, 1.0, L.z, R.z, B.z, 1.0, L.offset, R.offset, B.offset, 1.0
    Mat4x4 m_PlanesTNF;      // T.x, N.x, F.x, 1.0, T.y, N.y, F.y, 1.0, T.z, N.z, F.z, 1.0, T.offset, N.offset, F.offset, 1.0
    Vec4f m_PlaneX;          // x component of left,right,bottom,top
    Vec4f m_PlaneY;          // y component of left,right,bottom,top
    Vec4f m_PlaneZ;          // z component of left,right,bottom,top

    void BuildFrustrum(const Mat4x4& i_WorldMatrix, const Vec3f& i_CameraPosition, Float i_HRatio, Float i_VRatio, Float i_NearClip, Float i_FarClip);
};

struct Frustrum_Z {
    Mat4x4 m_WorldMatrix;
    Mat4x4 m_UnkMatrix;
    FrustrumPlane_Z m_ViewPlanes;      // Main view frustum planes
    FrustrumPlane_Z m_SecondaryPlanes; // Secondary frustum planes, most likely used for auxiliary visibility/occlusion-related tests
    Float m_NearClip;                  // Near clipping distance
    Float m_OccludedFarClip;           // Far clipping distance after occlusion adjustment
    Float m_FarClip;                   // Original far clipping distance
    Float m_BoundsSphereRadiusSq;      // Squared radius of a sphere enclosing the frustum bounds
    Vec3f m_BoundsSphereCenter;        // Center of that enclosing sphere, computed from the frustum AABB center
    Vec3f m_Direction;                 // Forward direction of the camera/frustum
    Vec3f m_Up;                        // Up vector of the camera/frustum, orthogonal to m_Direction and the right vector
    Vec3f m_BoundsMin;                 // Minimum corner of the frustum AABB in world space
    Vec3f m_BoundsMax;                 // Maximum corner of the frustum AABB in world space
    Vec2f m_TopViewBoundsMin;          // Minimum corner of the frustum bounds in top-view space
    Vec2f m_TopViewBoundsMax;          // Maximum corner of the frustum bounds in top-view space
    Vec3f m_WorldTranslation;          // World-space position of the frustum origin, usually the camera position
    Float m_FarPlaneHalfWidth;         // Half-width of the far plane
    Float m_FarPlaneHalfHeight;        // Half-height of the far plane
    Float m_FrustumFarClip;            // Effective far clip currently used by the frustum
    Vec3f m_CornerPoints[8];           // Frustum corner points in world space. 0..3 = near plane, 4..7 = far plane
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
    Float m_FovEdition; // Name from Monopoly
    Float m_UnkFloat_0x10d0;
    Float m_UnkFloat_0x10d4;
    Float m_UnkFloat_0x10d8;
    Float m_UnkFloat_0x10dc;

public:
    void UpdateInverseWorldMatrix(Node_Z* i_Node);

    virtual ~Camera_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void UpdateObject(Node_Z* a1, ObjectDatas_Z* a2);

    void DoOcclusion(const Occluder_ZHdl& i_OccluderHdl);
    void NoOcclusion();

    static BaseObject_Z* NewObject() { return NewL_Z(188) Camera_Z; }

    void SetFov(Float i_Fov);

    inline Float GetFov() const {
        return m_Fov;
    }

    void SetFovEdition(Float i_FovEdition);

    inline Float GetFovEdition() const {
        return m_FovEdition;
    }

    inline void SetRoll(Float i_Roll) {
        m_Roll = i_Roll;
    }

    void SetNearClip(Float i_Near);

    inline Float GetNearClip() const {
        return m_Frustum.m_NearClip;
    }

    void SetFarClip(Float i_Far);

    inline Float GetFarClip() const {
        return m_Frustum.m_FarClip;
    }

    void SetOccludedFarClip(Float i_Far);

    inline Float GetOccludedFarClip() const {
        // TODO
        return 0.f;
    }

    inline const Vec3f& GetDir() const {
        return m_Direction;
    }

    inline Mat4x4& GetInverseWorldMatrix() {
        return m_InverseWorldMatrix[gData.MatrixBuffer->GetCurBuffer()];
    }

    inline Frustrum_Z& GetFrustrum() {
        return m_Frustum;
    }

    inline const Vec3f& GetTarget() const {
        return m_Target;
    }

    inline const Vec3f& GetWorldPos() const {
        return m_WorldPos;
    }
};

#endif
