#include "Node_Z.h"
#include "MatrixCache_Z.h"

// TODO: Finish matching
Node_Z::Node_Z()
    : m_RotInWorld(1.0f, 0.0f, 0.0f, 0.0f)
    , m_Rotation(1.0f, 0.0f, 0.0f, 0.0f) {
    m_RotInWorldMatrix.SetIdentity();
    m_Parent = NULL;
    m_HeadSon = NULL;
    m_Prev = NULL;
    m_Next = NULL;
    m_Object = NULL;
    m_ObjectDatas = NULL;
    m_UserDefine = NULL;
    m_LightData = NULL;
    m_HFogData = NULL;
    m_RadiosityBitmap = NULL;
    m_RadiosityNormalBitmap = NULL;
    m_Flag = FL_NODE_COLLIDE;
    m_DisplaySeadRect.Set(-1, -1, -1, -1);
    m_CollideSeadRect.Set(-1, -1, -1, -1);
    SetCollideSeadId(-1);
    SetDisplaySeadId(-1);
    m_OccluderZoneStartId = 0;
    SetWorldId(-1);
    SetStart(0.0f);
    SetEnd(0.0f);
    m_Color.Set(1.0f, 1.0f, 1.0f, 1.0f);
    m_Scale = 1.0f;
    m_UniformScale = 1.0f;
    m_InverseUniformScale = 1.0f;
    m_BSphere.Set(VEC3F_NULL, -1.0f);
    SetTranslation(VEC3F_NULL);
    SetWorldMatrixId(gData.MatrixBuffer->GetNewMatrix());
    SetWorldMatrixPtr();
}

void Node_Z::SetTranslation(const Vec3f& i_Translation) {
    m_Translation = i_Translation;
}
