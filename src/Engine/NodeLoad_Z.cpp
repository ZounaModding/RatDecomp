#include "Node_Z.h"
#include "Program_Z.h"

// TODO: Finish matching
void Node_Z::Load(void** i_Data) {
    U16 l_U16;
    S16 l_S16;
    Float l_Float;

    LOADLINKID_Z(&m_Parent);
    LOADLINKID_Z(&m_HeadSon);
    LOADLINKID_Z(&m_Prev);
    LOADLINKID_Z(&m_Next);
    LOADLINKID_Z(&m_Object);
    LOADLINKID_Z(&m_UserDefine);
    LOADLINKID_Z(&m_RadiosityBitmap);
    LOADLINKID_Z(&m_RadiosityNormalBitmap);
    l_U16 = GetWorldMatrixId();
    LOADRANGE_Z(m_InverseWorldMatrix, m_CollideSeadRect);
    SetWorldMatrixId(l_U16);
    SetWorldMatrixPtr();
    LOAD_Z(GetWorldMatrix());
    LOAD_Z(GetCollideSeadId());
    LOAD_Z(GetDisplaySeadId());
    LOAD_Z(l_S16);
    SetWorldId(l_S16);
    LOAD_Z(l_Float);
    SetStart(l_Float);
    LOAD_Z(l_Float);
    SetEnd(l_Float);
}

void Node_Z::EndLoad() {
    ResourceObject_Z::EndLoad();
    gData.ClassMgr->UpdateLinkPtr(&m_Parent);
    gData.ClassMgr->UpdateLinkPtr(&m_HeadSon);
    gData.ClassMgr->UpdateLinkPtr(&m_Prev);
    gData.ClassMgr->UpdateLinkPtr(&m_Next);
    gData.ClassMgr->UpdateLinkPtr(&m_Object);
    gData.ClassMgr->UpdateLinkPtr(&m_UserDefine);
    gData.ClassMgr->UpdateLinkPtr(&m_RadiosityBitmap);
    gData.ClassMgr->UpdateLinkPtr(&m_RadiosityNormalBitmap);
}

void Node_Z::AfterEndLoad() {
    BuildNonShared(FALSE, FALSE);
}
