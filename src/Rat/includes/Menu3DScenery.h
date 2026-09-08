#ifndef _MENU3DSCENERY_H_
#define _MENU3DSCENERY_H_
#include "Types_Z.h"

class MenuManager_G;

class Menu3DScenery {
public:
    U8 m_Unk_0x000[0xfc];
    S32 m_AnimationId;
    U8 m_Unk_0x100[0x10];
};

class Book : public Menu3DScenery {
public:
    Book(MenuManager_G* i_MenuManager);
};

#endif // _MENU3DSCENERY_H_
