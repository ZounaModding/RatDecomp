#ifndef _MATERIALANIM_Z_H_
#define _MATERIALANIM_Z_H_
#include "ResourceObject_Z.h"
#include "Keyframer_Z.h"
#include "Material_ZHdl.h"
#include "Material_Z.h"

class MaterialAnim_ZHdl;

#define FL_MTL_NONE (U8)(0 << 0)
#define FL_MTL_PLAY (U8)(1 << 0)
#define FL_MTL_PLAYED (U8)(1 << 1)
#define FL_MTL_PLAYONCE (U8)(1 << 2)
#define FL_MTL_NEVERAGAIN (U8)(1 << 3)
#define FL_MTL_AUTOSTART (U8)(1 << 4)
#define FL_MTL_NOT_ANIMATED (U8)(1 << 5)

#define FL_MTL_NOT_RESTART (U8)(FL_MTL_PLAYONCE | FL_MTL_PLAYED)

class MaterialAnim_Z : public ResourceObject_Z {
public:
    MaterialAnim_Z();

    virtual ~MaterialAnim_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();
    virtual void Clone(MaterialAnim_ZHdl& i_MaterialAnimHdl);

    static BaseObject_Z* NewObject() { return NewL_Z(93) MaterialAnim_Z; }

    void Reset();
    void DoStart();
    void Stop();
    void Update(Float i_DeltaTime);
    void SetAnimTime(Float i_Time);

    inline void SetMaterial(const Material_ZHdl& i_MaterialHdl) {
        m_MaterialHdl = i_MaterialHdl;
    }

    inline const Material_ZHdl& GetMaterial() const {
        return m_MaterialHdl;
    }

    Bool IsAnimated() {
        return !(m_PlayFlag & FL_MTL_NOT_ANIMATED);
    }

private:
    U8 m_PlayFlag;
    KeyframerHdl_Z m_BmapHdlKfr;
    KeyframerVec2fLinear_Z m_TransKfr;
    KeyframerVec2fLinear_Z m_ScaleKfr;
    KeyframerFloatLinearComp_Z m_RotKfr;
    KeyframerVec3fLinear_Z m_DiffuseColorKfr;
    KeyframerVec3fLinear_Z m_EmissiveColorKfr;
    KeyframerFloatLinearComp_Z m_DiffuseAlphaKfr;
    KeyframerVec4fLinear_Z m_SpecularKfr;
    KeyframerVec4fLinear_Z m_ParamsKfr;
    KeyframerFlag_Z m_CollisionFlagKfr;
    KeyframerFlag_Z m_RenderFlagKfr;
    KeyframerFlag_Z m_ObjectFlagKfr;
    Float m_CurTime;
    Float m_MaxTime;
    Material_ZHdl m_MaterialHdl;
};

#endif // _MATERIALANIM_Z_H_
