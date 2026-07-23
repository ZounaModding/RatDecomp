#ifndef _FLARE_Z_H_
#define _FLARE_Z_H_
#include "Object_Z.h"

class Flare_Z : public Object_Z {
public:
    Flare_Z();

    virtual ~Flare_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void GetFlareParams(DrawInfo_Z& i_DrawInfo, Vec4f& a2, Vec4f& a3, Float* a4, Float* a5, Float* a6);

protected:
    U8 m_Pad_0x80[16];
};

#endif // _FLARE_Z_H_
