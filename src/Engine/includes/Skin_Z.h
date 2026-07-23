#ifndef _SKIN_Z_H_
#define _SKIN_Z_H_
#include "Object_Z.h"

class Skin_Z : public Object_Z {
public:
    Skin_Z();
    virtual ~Skin_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual Bool GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionMovingSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);

    virtual void UpdateBSphere(const ObjectDatas_ZHdl& i_DataHdl) { }

protected:
    U8 m_Pad_0x80[24];
};

#endif // _SKIN_Z_H_
