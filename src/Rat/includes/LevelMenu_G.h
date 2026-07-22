#ifndef _LEVELMENU_G_H_
#define _LEVELMENU_G_H_
#include "DynArray_Z.h"
#include "Types_Z.h"

struct LevelMenu_G {
    U8 m_Data[0x6c];
};

struct LevelDemoMenu_G {
    U8 m_Data[0x6c];
};

struct MenuMpegText {
    U8 m_Data[0x10];
};

struct MultiGame {
    U8 m_Data[0x18];
};

struct Championship {
    U8 m_Data[0x18];
};

typedef DynArray_Z<LevelMenu_G, 4> LevelMenu_GDA;
typedef DynArray_Z<LevelDemoMenu_G, 4> LevelDemoMenu_GDA;
typedef DynArray_Z<MenuMpegText, 1> MenuMpegTextDA;
typedef DynArray_Z<MultiGame, 1> MultiGameDA;
typedef DynArray_Z<Championship, 1> ChampionshipDA;
#endif // _LEVELMENU_G_H_
