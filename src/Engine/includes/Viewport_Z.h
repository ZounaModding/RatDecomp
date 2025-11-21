#ifndef _VIEWPORT_Z_H_
#define _VIEWPORT_Z_H_
#include "Handle_Z.h"
#include "Assert_Z.h"
#include "GCMain_Z.h"
#include "ManipulatorDraw_ZHdl.h"
#include "Camera_Z.h"
#include "Node_ZHdl.h"
class Renderer_Z;

class Viewport_Z {
public:
    void SetPosAndSize(S32 i_StartX, S32 i_StartY, S32 i_SizeX, S32 i_SizeY) {
        m_StartX = i_StartX;
        m_StartY = i_StartY;
        m_SizeX = i_SizeX;
        m_SizeY = i_SizeY;
    }

    void SetRenderer(Renderer_Z* i_Renderer) {
        m_Renderer = i_Renderer;
    }

    Float GetHSize() const {
        return m_HSize;
    }

    Float GetVSize() const {
        return m_VSize;
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

    Viewport_Z();
    ~Viewport_Z();
    void Init();
    U32 m_StartX;
    U32 m_StartY;
    U32 m_SizeX;
    U32 m_SizeY;
    BaseObject_ZHdl m_WorldHdl;
    BaseObject_ZHdl m_OccluderHdl;
    Node_ZHdl m_CameraNodeHdl;
    Float m_Tangent;
    Float m_InvDiagTangent;
    Float m_DFov;
    Float m_Ratio;
    Float m_HRatio;
    Float m_VRatio;
    Float m_HSize;
    Float m_VSize;
    Float m_HCenter;
    Float m_VCenter;
    Float m_UnkFloat0x44;
    Float m_UnkFloat0x48;
    Float m_UnkFloat0x4C;
    Float m_UnkFloat0x50;
    Float m_UnkFloat0x54;
    Float m_UnkFloat0x58;
    Float m_UnkFloat0x5C;
    Float m_UnkFloat0x60;
    Float m_UnkFloat0x64;
    Float m_UnkFloat0x68;
    Float m_UnkFloat0x6C;
    Float m_UnkFloat0x70;
    Float m_UnkFloat0x74;
    Float m_UnkFloat0x78;
    Float m_UnkFloat0x7C;
    Float m_UnkFloat0x80;
    Float m_UnkFloat0x84;
    Float m_UnkFloat0x88;
    U16 m_IdxInMat4x4Buffer;
    ManipulatorDraw_ZHdl* ManipDrawHdls[16];
    S32 m_NumOfManipDraws;
    Char m_UnkBytes0xD4[12];
    Renderer_Z* m_Renderer;
    U8 m_UnkBytes0xE4[12];
};

#endif
