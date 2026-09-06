#ifndef _CAMERA_Z_H_
#define _CAMERA_Z_H_
#include "Object_Z.h"
#include "Node_ZHdl.h"
#include "MatrixCache_Z.h"
#include "Occluder_ZHdl.h"

// $SABE: This is a bit hacky, but the constructor can't match
//        if m_PlanesLRB and m_PlanesTNF Vec4f arrays, so eh
struct FrustrumVec4_Z {
    Float x;
    Float y;
    Float z;
    Float w;

    void Set(Float i_X, Float i_Y, Float i_Z, Float i_W) {
        x = i_X;
        y = i_Y;
        z = i_Z;
        w = i_W;
    }
} Aligned_Z(16);

struct FrustrumPlane_Z {
    Vec4f m_PlaneNormals[6];       // plane normals - left,right,bottom,top,near,far I think
    Vec4f m_PlanesDir[6];          // plane normals transformed by world matrix WITHOUT translation
    Float m_PlaneOffsets[6];       // the D (or -D im not sure) of the planes (you can move up the normal by this to get the real world coord plane)
    FrustrumVec4_Z m_PlanesLRB[4]; // L.x, R.x, B.x, 1.0, L.y, R.y, B.y, 1.0, L.z, R.z, B.z, 1.0, L.offset, R.offset, B.offset, 1.0
    FrustrumVec4_Z m_PlanesTNF[4]; // T.x, N.x, F.x, 1.0, T.y, N.y, F.y, 1.0, T.z, N.z, F.z, 1.0, T.offset, N.offset, F.offset, 1.0
    Vec4f m_PlaneX;                // x component of left,right,bottom,top
    Vec4f m_PlaneY;                // y component of left,right,bottom,top
    Vec4f m_PlaneZ;                // z component of left,right,bottom,top

    void BuildFrustrum(const Mat4x4& i_WorldMatrix, const Vec3f& i_CameraPosition, Float i_HRatio, Float i_VRatio, Float i_NearClip, Float i_FarClip);
    void BuildPlane(const Vec3f& i_CameraPosition, Float i_NearClip, Float i_FarClip);
};

#define FRUST_CORNER_TOP_RIGHT_NEAR 0
#define FRUST_CORNER_BOTTOM_RIGHT_NEAR 1
#define FRUST_CORNER_BOTTOM_LEFT_NEAR 2
#define FRUST_CORNER_TOP_LEFT_NEAR 3
#define FRUST_CORNER_TOP_RIGHT_FAR 4
#define FRUST_CORNER_BOTTOM_RIGHT_FAR 5
#define FRUST_CORNER_BOTTOM_LEFT_FAR 6
#define FRUST_CORNER_TOP_LEFT_FAR 7

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
    Float m_FarPlaneData[3];           // Half-width, half-height, and effective far clip
    Vec3f m_CornerPoints[8];           // Frustum corner points in world space. 0..3 = near plane, 4..7 = far plane
};

struct Frustrum2D_Z {
    S32 m_PointNb;
    Vec2f m_Points[6];
};

#define MAX_OCCLUDED_FRUSTUM_EDGES 96

// Represents the top-down projected frustum after occlusion (it's actually a convex polygon)
struct OccludedFrustum_Z {
    // Vector from camera pos to target pos
    Vec2f m_CamToTarget;
    // Outer points of the polygon in top-down space (XZ). At [0] is the camera pos
    // These go from left to right in clockwise order, and start as the 2d projection of the camera frustum corners.
    // Points may be added later to the polygon if occluders are found, and the polygon is clipped against them.
    //
    // Example of a 5-point frustum:              Example of a 3-point frustum:
    //
    //         2---------3
    //         |         |                                 1---------2
    //         |         |                                  \       /
    //         1         4                                   \     /
    //          \       /                                     \   /
    //           \     /                                        0
    //            \   /
    //              0
    //
    Vec2f m_Points[MAX_OCCLUDED_FRUSTUM_EDGES + 1];
    // 0.0-1.0 float from left to right in screen space (for each point)
    Float m_HorizontalProjection[MAX_OCCLUDED_FRUSTUM_EDGES + 1];
    // Difference between point at i and i+1
    Vec2f m_EdgeVectors[MAX_OCCLUDED_FRUSTUM_EDGES];
    // Difference between the camera point and the point at i
    Vec2f m_PointOffsetsFromOrigin[MAX_OCCLUDED_FRUSTUM_EDGES + 1];
    // Camera pos in 3D world space
    Vec3f m_WorldPos;
    // Camera target pos in 3D world space
    Vec3f m_TargetWorldPos;
    S32 m_PointNb;
    // Segment start pos (XZ) for screen space projection of points
    Vec2f m_ProjectionLineStart;
    // Vector from m_ProjectionLineStart to projection end
    Vec2f m_ProjectionLineDelta;
    // Has the frustum been occluded by an occluder?
    Bool m_IsOccluded;

    const Vec2f& GetCamPos() const {
        return m_Points[0];
    }

    void DoVec() {
        m_Points[m_PointNb] = m_Points[0];
        for (S32 i = 0; i < m_PointNb; i++) {
            m_EdgeVectors[i] = m_Points[i + 1] - m_Points[i];
        }
        for (S32 i = 0; i < m_PointNb + 1; i++) {
            m_PointOffsetsFromOrigin[i] = m_Points[i] - m_Points[0];
        }
    }

    OccludedFrustum_Z& operator=(const OccludedFrustum_Z& i_Other) {
        m_PointNb = i_Other.m_PointNb;
        m_CamToTarget = i_Other.m_CamToTarget;
        m_WorldPos = i_Other.m_WorldPos;
        m_TargetWorldPos = i_Other.m_TargetWorldPos;

        for (S32 i = 0; i < m_PointNb; i++) {
            m_EdgeVectors[i] = i_Other.m_EdgeVectors[i];
            m_Points[i] = i_Other.m_Points[i];
            m_HorizontalProjection[i] = i_Other.m_HorizontalProjection[i];
        }

        m_HorizontalProjection[m_PointNb] = i_Other.m_HorizontalProjection[m_PointNb];
        m_Points[m_PointNb] = m_Points[0];
        m_ProjectionLineStart = i_Other.m_ProjectionLineStart;
        m_ProjectionLineDelta = i_Other.m_ProjectionLineDelta;
        return *this;
    }

    Bool GetPtsOnLineX(Float i_X, FloatDA& o_IntersectionsZ) const;
    // Should be called GetPtsOnLineZ
    Bool GetPtsOnLineY(Float i_Z, FloatDA& o_IntersectionsX) const;
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
    Occluder_ZHdl m_OccluderHdl;
    BitArray_Z m_OccludedZonesBA; // possibly bit array of occluded zones
    OccludedFrustum_Z m_OccludedFrustum;
    Float m_FovEdition; // Name from Monopoly
    Float m_AverageGrayLevel;
    Float m_WhiteLevel;
    Float m_UnkFloat_0x10d8;
    Float m_UnkFloat_0x10dc;

public:
    Camera_Z();

    void UpdateInverseWorldMatrix(Node_Z* i_Node);

    virtual ~Camera_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void UpdateObject(Node_Z* a1, ObjectDatas_Z* a2);

    void DoOcclusion(const Occluder_ZHdl& i_OccluderHdl);
    void NoOcclusion();
    void GetOccluded(DrawInfo_Z& io_DrawInfo);
    void GetFrustrum2D(Frustrum2D_Z& o_Frustrum) const;

    static BaseObject_Z* NewObject() { return NewL_Z(188) Camera_Z; }

    void SetFov(Float i_Fov);
    void SetAverageGrayLevel(Float i_AverageGrayLevel);
    void SetWhiteLevel(Float i_WhiteLevel);
    void SetTarget(const Vec3f& i_Target);

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

    inline const OccludedFrustum_Z& GetOccludedFrustum() const {
        return m_OccludedFrustum;
    }

    inline const BitArray_Z& GetOccludedZones() const {
        return m_OccludedZonesBA;
    }
};

#endif
