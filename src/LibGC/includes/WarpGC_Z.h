#ifndef _WARPGC_Z_H_
#define _WARPGC_Z_H_
#include "Warp_Z.h"
#include "DisplayList_Z.h"

class WarpGC_Z : public Warp_Z {
public:
    WarpGC_Z();
    virtual ~WarpGC_Z();
    virtual void EndLoad();
    virtual void Draw(DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() {
        return NewL_Z(30) WarpGC_Z;
    }

private:
    void BuildDisplayList();

    U8 m_Unk_0xc8[24];
    Vec2f m_TextureCoordinates[4];
    ByteColor m_Colors[6];
    DisplayList_Z m_DisplayLists[6];
    U8 m_Unk_0x178[8];
    Mat4x4 m_Projection;
};

#endif // _WARPGC_Z_H_
