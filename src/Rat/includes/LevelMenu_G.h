#ifndef _LEVELMENU_G_H_
#define _LEVELMENU_G_H_
#include "DynArray_Z.h"
#include "LevelData_GHdl.h"
#include "Name_Z.h"
#include "Types_Z.h"

struct LevelMenu_G {
    U8 m_Data[0x6c];
};

struct LevelDemoMenu_G {
    U8 m_Data[0x6c];
};

struct MenuMpegText {
    MenuMpegText() { }

    U8 m_Data[0x10];
};

template <class T>
struct Multi {
    Name_Z m_Name;
    S32 m_Unk_0x04;
    S32 m_Unk_0x08;
    DynArray_Z<T, 2> m_Levels;
};

struct MultiGame : public Multi<LevelData_GHdl> {
    S32 GetIAPlayers() { return m_IAPlayers; }

    S32 m_IAPlayers;
};

struct Championship : public Multi<Name_Z> {
    S32 m_Unk_0x14;
};

typedef DynArray_Z<LevelMenu_G, 4> LevelMenu_GDA;
typedef DynArray_Z<LevelDemoMenu_G, 4> LevelDemoMenu_GDA;
typedef DynArray_Z<MenuMpegText, 1> MenuMpegTextDA;
typedef DynArray_Z<MultiGame, 1> MultiGameDA;
typedef DynArray_Z<Championship, 1> ChampionshipDA;
#endif // _LEVELMENU_G_H_
