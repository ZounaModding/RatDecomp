#ifndef _FLARE_Z_H_
#define _FLARE_Z_H_
#include "Object_Z.h"
#include "Material_ZHdl.h"
#include "MaterialAnim_ZHdl.h"

struct FlareDesc_Z {
    Vec4f m_Color;
    Float m_Scale;
    Float m_Position;
    Vec2f m_UVs[4];
    Material_ZHdl m_MaterialHdl;
    MaterialAnim_ZHdl m_MaterialAnimHdl;

    FlareDesc_Z() {
        m_Scale = 1.0f;
        m_Position = 0.5f;
    }
};

typedef DynArray_Z<FlareDesc_Z, 32, FALSE> FlareDesc_ZDA;

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

    void GetFarPos(const Vec3f& i_Pos, Float i_FarClip, Vec3f& o_FarPos);

protected:
    Bool m_Active;
    FlareDesc_ZDA m_Flares;
};

#endif // _FLARE_Z_H_
