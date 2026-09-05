#ifndef _VIEWPORT_Z_H_
#define _VIEWPORT_Z_H_
#include "Handle_Z.h"
#include "Assert_Z.h"
#include "ManipulatorDraw_ZHdl.h"
#include "Node_ZHdl.h"
#include "World_ZHdl.h"
#include "Occluder_ZHdl.h"
#include "StaticArray_Z.h"
#include "Math_Z.h"
#include "Camera_Z.h"
#include "Node_Z.h"

#define FL_CLIPPLANE_NEAR (1 << 0)
#define FL_CLIPPLANE_SIDES (1 << 1)
#define FL_CLIPPLANE_FAR (1 << 2)

Bool WSphereVsFrustrum(const Frustrum_Z& i_Frustrum, const Sphere_Z& i_Sphere, U32& o_Flag);
Bool SphereVsFrustrum(const Frustrum_Z& i_Frustrum, const Sphere_Z& i_Sphere, U32& o_Flag);
Bool SphereVsFrustrum(const Frustrum_Z& i_Frustrum, DrawInfo_Z& io_DrawInfo, Node_Z* i_Node);
Bool SphereVsFrustrum(const Frustrum_Z& i_Frustrum, const Mat4x4& i_Matrix, const Sphere_Z& i_Sphere, ClipSphere_Z& o_ClipSphere, Float i_Scale);
Bool BoxVsFrustrumAndFlags(const Frustrum_Z& i_Frustrum, const Mat4x4& i_Matrix, const Box_Z& i_Box, U32& o_Flag, Float i_Scale);

class Renderer_Z;

class Viewport_Z {
public:
    Viewport_Z();
    ~Viewport_Z();
    void Init();

    void SetPosAndSize(S32 i_StartX, S32 i_StartY, S32 i_SizeX, S32 i_SizeY);

    void GetPosAndSize(S32& o_StartX, S32& o_StartY, S32& o_SizeX, S32& o_SizeY) const;

    void SetRenderer(Renderer_Z* i_Renderer) {
        m_Renderer = i_Renderer;
    }

    Renderer_Z* GetRenderer() const {
        return m_Renderer;
    }

    Float GetHSize() const {
        return m_HSize;
    }

    Float GetVSize() const {
        return m_VSize;
    }

    Float GetDFov() const {
        ASSERTLE_Z(m_DFov >= 0.0f, "", 67, "DFov>=0.f");
        return m_DFov;
    }

    Float GetTang() const {
        ASSERTLE_Z(m_Tangent >= 0.0f, "", 68, "Tang>=0.f");
        return m_Tangent;
    }

    Float GetInvDTang() const {
        ASSERTLE_Z(m_InvDiagTangent >= 0.0f, "", 69, "InvDTang>=0.f");
        return m_InvDiagTangent;
    }

    Float GetRatio() const {
        ASSERTLE_Z(m_Ratio > 0.0f, "", 70, "Ratio>0.f");
        return m_Ratio;
    }

    Float GetHRatio() const {
        ASSERTLE_Z(m_HRatio > 0.0f, "", 71, "HRatio>0.f");
        return m_HRatio;
    }

    Float GetVRatio() const {
        ASSERTLE_Z(m_VRatio > 0.0f, "", 72, "VRatio>0.f");
        return m_VRatio;
    }

    Float GetHCenter() const {
        ASSERTLE_Z(m_HCenter > 0.0f, "", 75, "HCenter>0.f");
        return m_HCenter;
    }

    Float GetVCenter() const {
        ASSERTLE_Z(m_VCenter > 0.0f, "", 76, "VCenter>0.f");
        return m_VCenter;
    }

    const Node_ZHdl& GetCamera() const {
        return m_CameraNodeHdl;
    }

    inline const Vec3f& GetCameraPos() const {
        return ((Camera_Z*)m_CameraNodeHdl->GetObjectA())->GetWorldPos();
    }

    void SetWorld(const World_ZHdl& i_WorldHdl);
    void SetCamera(const Node_ZHdl& i_CameraNodeHdl);

    const World_ZHdl& GetWorld() const {
        return m_WorldHdl;
    }

    const Occluder_ZHdl& GetOccluder() const {
        return m_OccluderHdl;
    }

    void RegisterManip(const ManipulatorDraw_ZHdl& i_ManipDrawHdl);
    void UnregisterManip(const ManipulatorDraw_ZHdl& i_ManipDrawHdl);
    void UpdateFrustrum();
    void Draw(DrawInfo_Z& i_DrawInfo);
    const Mat4x4& GetMatrixInv() const;

private:
    S32 m_StartX;
    S32 m_StartY;
    S32 m_SizeX;
    S32 m_SizeY;
    World_ZHdl m_WorldHdl;
    Occluder_ZHdl m_OccluderHdl;
    Node_ZHdl m_CameraNodeHdl;
    Float m_Tangent;          // tan(FOV / 2)
    Float m_InvDiagTangent;   // Ratio between the alternate/edit FOV tangent and m_Tangent
    Float m_DFov;             // Projection plane distance derived from viewport width and FOV
    Float m_Ratio;            // Aspect ratio normalized against 4:3. 1.0f = 4:3, > 1.0f = wider, < 1.0f = narrower
    Float m_HRatio;           // Horizontal frustum half-extent at unit distance
    Float m_VRatio;           // Vertical frustum half-extent at unit distance
    Float m_HSize;            // Horizontal projection scale or derived horizontal projection size
    Float m_VSize;            // Vertical projection scale or derived vertical projection size
    Float m_HCenter;          // Horizontal viewport center or projection center offset
    Float m_VCenter;          // Vertical viewport center or projection center offset
    Vec3f m_PlaneDirL;        // World space direction left frustum plane
    Vec3f m_PlaneDirR;        // World space direction right frustum plane
    Vec3f m_PlaneDirB;        // World space direction bottom frustum plane
    Vec3f m_PlaneDirT;        // World space direction top frustum plane
    Vec3f m_FrustumBoundsMin; // Minimum corner of the viewport frustum AABB in world space
    Vec3f m_FrustumBoundsMax; // Maximum corner of the viewport frustum AABB in world space
    U16 m_CameraMatrixId;     // Index of camera matrix in Mat4x4Buffer_Z
    StaticArray_Z<ManipulatorDraw_ZHdl, 16> m_ManipDrawHdls;
    Renderer_Z* m_Renderer;
};

#endif
