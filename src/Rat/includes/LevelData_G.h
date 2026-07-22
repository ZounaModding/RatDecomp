#ifndef _LEVELDATA_G_H_
#define _LEVELDATA_G_H_
#include "BaseObject_Z.h"

struct MaterialLib {
    Name_Z m_Name;
    S32 m_TextId;
    U8 m_Data[0x4c];
};

class LevelData_G : public BaseObject_Z {
public:
    MaterialLib* GetMaterialLib(const Name_Z& i_Name);
    void ResetAdvancement();

private:
    U8 m_Unk_0x0c[0xc4];
};
#endif // _LEVELDATA_G_H_
