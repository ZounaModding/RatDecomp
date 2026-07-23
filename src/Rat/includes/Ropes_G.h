#ifndef _ROPES_G_H_
#define _ROPES_G_H_
#include "Manipulator_Z.h"

class Ropes_G : public ManipulatorSceneDraw_Z {
public:
    virtual ~Ropes_G() { }

    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() { return NewL_Z(140) Ropes_G; }

    void Minimize();

private:
    U8 m_Unk_0x20[0xc];
};
#endif // _ROPES_G_H_
