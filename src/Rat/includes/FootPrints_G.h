#ifndef _FOOTPRINTS_G_H_
#define _FOOTPRINTS_G_H_
#include "Manipulator_Z.h"

class FootPrints_G : public ManipulatorSceneDraw_Z {
public:
    virtual ~FootPrints_G();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() { return NewL_Z(53) FootPrints_G; }

    void Minimize();

private:
    U8 m_Unk_0x20[0x1c10];
};
#endif // _FOOTPRINTS_G_H_
