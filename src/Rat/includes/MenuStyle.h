#ifndef _MENUSTYLE_H_
#define _MENUSTYLE_H_
#include "Name_Z.h"
#include "Types_Z.h"

class styleBitmap {
public:
    void SetMaterials();
    void SetAll(Name_Z i_Name, Float i_X, Float i_Y, Float i_Width, Float i_Height);
    void SetAllDimensions(Float i_X, Float i_Y, Float i_Width, Float i_Height);

private:
    U8 m_MenuStyle[0x94];
};

#endif // _MENUSTYLE_H_
