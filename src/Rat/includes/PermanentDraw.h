#ifndef _PERMANENTDRAW_H_
#define _PERMANENTDRAW_H_
#include "BaseInGameDatas_G.h"

class PermanentDraw : public BaseInGameDatas_G {
public:
    PermanentDraw() { }

    virtual ~PermanentDraw() { }

    virtual void Init();

    virtual void Update(Float i_DeltaTime) { }

    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() { return NewL_Z(24) PermanentDraw; }

private:
    U8 m_Unk_0x12b4[0x10];
};
#endif // _PERMANENTDRAW_H_
