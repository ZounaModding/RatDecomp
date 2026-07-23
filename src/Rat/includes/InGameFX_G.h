#ifndef _INGAMEFX_G_H_
#define _INGAMEFX_G_H_
#include "DynArray_Z.h"
#include "Manipulator_Z.h"
#include "Node_ZHdl.h"
#include "Omni_ZHdl.h"

#define FX_OMNI_BLINK_FIRST_END 0.05f
#define FX_OMNI_BLINK_SECOND_START 0.1f
#define FX_OMNI_BLINK_SECOND_END 0.2f
#define FX_OMNI_BLINK_FINAL_START 0.25f

enum FXOmniInterpType {
    FX_OMNI_INTERP_LINEAR,
    FX_OMNI_INTERP_SINE,
    FX_OMNI_INTERP_DOUBLE_SINE,
    FX_OMNI_INTERP_DOUBLE_SINE_BLINK
};

class InGameFX_G : public ManipulatorSceneDraw_Z {
public:
    virtual ~InGameFX_G() { }

    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() { return NewL_Z(11) InGameFX_G; }
};

struct FXOmni {
    Omni_ZHdl m_OmniHdl;
    Node_ZHdl m_NodeHdl;
    Vec3f m_Color;
    Float m_Time;
    Float m_Duration;
    FXOmniInterpType m_Type;
    Bool m_IsActive;
};

typedef DynArray_Z<FXOmni, 8> FXOmniDA;
#endif // _INGAMEFX_G_H_
