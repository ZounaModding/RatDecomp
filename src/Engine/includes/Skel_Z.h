#ifndef _SKEL_Z_H_
#define _SKEL_Z_H_
#include "Object_Z.h"
#include "Camera_Z.h"

class Skel_Z : public ObjectDatas_Z {
public:
    Skel_Z();
    virtual ~Skel_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual Bool MarkHandles();
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual void IsBSphere(Node_Z* i_Node, Frustrum_Z& i_Frust, DrawInfo_Z& i_DrawInfo);
    virtual void GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);

    S32 HideObjectNamed(const Name_Z& i_ObjName, Bool i_AlsoSomethingElse);
    S32 UnhideObjectNamed(const Name_Z& i_ObjName, Bool i_AlsoSomethingElse);

protected:
    U8 m_Pad_0x28[104];
};

#endif // _SKEL_Z_H_
