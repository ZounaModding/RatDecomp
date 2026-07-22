#ifndef _LIQUIDFLOW_G_H_
#define _LIQUIDFLOW_G_H_
#include "Manipulator_Z.h"

class LiquidFlow_G : public ManipulatorSceneDraw_Z {
public:
    virtual ~LiquidFlow_G();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() { return NewL_Z(183) LiquidFlow_G; }

    void Minimize();

private:
    U8 m_Unk_0x20[0x14];
};
#endif // _LIQUIDFLOW_G_H_
