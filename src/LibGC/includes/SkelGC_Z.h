#ifndef _SKELGC_Z_H_
#define _SKELGC_Z_H_
#include "Skel_Z.h"
#include "StreamList_Z.h"

class SkinStreamList_Z : public StreamList_Z {
public:
    virtual void SetStream();
    virtual void SetVtxDesc();

    U32 m_Unk_0x0[3];
};

class SkelGC_Z : public Skel_Z {
public:
    SkelGC_Z();
    virtual ~SkelGC_Z();
    virtual void AfterEndLoad();
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual void Draw(DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() {
        return NewL_Z(75) SkelGC_Z;
    }

private:
    void InitConstant();

    DynArray_Z<Mat3x3, 32, FALSE, FALSE, 4> m_ConstantMatrices;
    S16DA m_BoneToConstantIndices;
    U32 m_LastFrameUpdated;
    DynArray_Z<SkinStreamList_Z, 32, TRUE, TRUE, 4> m_StreamLists;
};

#endif // _SKELGC_Z_H_
