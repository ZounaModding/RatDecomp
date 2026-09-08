#ifndef _FRAME_G_H_
#define _FRAME_G_H_
#include "Types_Z.h"

class Dialog_G;
class MenuManager_G;

class Frame_G {
public:
    Bool Is(S32 i_Type) { return m_Type == i_Type; }

    void SetType(S32 i_Type) { m_Type = i_Type; }

    void SetMenuManager(MenuManager_G* i_MenuManager) { m_MenuManager = i_MenuManager; }

    void SetDialog(Dialog_G* i_Dialog) { m_Dialog = i_Dialog; }

    void SetPosAndSize(Float i_PosX, Float i_PosY, Float i_SizeX, Float i_SizeY) {
        m_PosX = i_PosX;
        m_PosY = i_PosY;
        m_SizeX = i_SizeX;
        m_SizeY = i_SizeY;
    }

private:
    S32 m_Type;
    U8 m_Unk_0x04[4];
    MenuManager_G* m_MenuManager;
    Dialog_G* m_Dialog;
    Float m_PosX;
    Float m_PosY;
    Float m_SizeX;
    Float m_SizeY;
    U8 m_Unk_0x20[12];
};
#endif // _FRAME_G_H_
