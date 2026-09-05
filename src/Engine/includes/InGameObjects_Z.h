#ifndef _INGAMEOBJECTS_Z_H_
#define _INGAMEOBJECTS_Z_H_
#include "Manipulator_Z.h"
#include "Node_ZHdl.h"
#include "Camera_ZHdl.h"

class InGameObjects_Z : public ManipulatorDraw_Z {
public:
    InGameObjects_Z();
    virtual void Init();
    virtual ~InGameObjects_Z();
    virtual Bool MarkHandles();
    virtual void Draw(const Viewport_Z* i_Viewport);
    virtual void SetLights(Node_ZHdl& i_LightNodeHdl1, Node_ZHdl& i_LightNodeHdl2, Node_ZHdl& i_LightNodeHdl3, Node_ZHdl& i_LightNodeHdl4);
    virtual void LoadDataDesc(const Char* i_Path);

    void SetRoot(Node_ZHdl& i_NodeHdl);
    void GetAllNodes();
    void SetNearClip(Float i_Near);
    void SetFarClip(Float i_Far);
    void SetFov(Float i_Fov);
    void SetPosAndSize(Float i_PosX, Float i_PosY, Float i_SizeX, Float i_SizeY);

protected:
    Float m_PosX;
    Float m_PosY;
    Float m_SizeX;
    Float m_SizeY;
    Node_ZHdl m_RootNodeHdl;
    Node_ZHdlDA m_Nodes;
    Node_ZHdl m_CameraNodeHdl;
    Camera_ZHdl m_CameraHdl;
    U8 m_Pad_0x48[28];
    Node_ZHdl m_LightNodeHdl;
    Node_ZHdl m_LightNodeHdls[3];
    Node_ZHdl m_UnkNodeHdl_0x74;
};
#endif // _INGAMEOBJECTS_Z_H_
