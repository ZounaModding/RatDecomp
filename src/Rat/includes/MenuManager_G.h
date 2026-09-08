#ifndef _MENUMANAGER_G_H_
#define _MENUMANAGER_G_H_
#include "BaseInGameDatas_G.h"
#include "Menu3DScenery.h"
#include "MenuStyle.h"
#include "Name_Z.h"
#include "Types_Z.h"

class Dialog_G;

class MenuManager_G : public BaseInGameDatas_G {
public:
    virtual void Init();
    void ResetStyles();
    void InitVoices();
    void ResetKeys();

    void UnHideMouse() { m_HideMouse = FALSE; }

    void SetMenu3DScenery(Menu3DScenery* i_Scenery) { m_Menu3DScenery = i_Scenery; }

    void SetRessourcesParsed(Bool i_Parsed);
    styleBox* GetStyleBoxByName(const Name_Z& i_Name);
    S32 GetIdStyleBox(styleBox* i_Style);
    styleText* GetStyleTextByName(const Name_Z& i_Name);
    S32 GetIdStyleText(styleText* i_Style);
    styleBitmap* GetStyleBitmapByName(const Name_Z& i_Name);
    S32 GetIdStyleBitmap(styleBitmap* i_Style);
    styleBitmapColor* GetStyleBitmapColorByName(const Name_Z& i_Name);
    S32 GetIdStyleBitmapColor(styleBitmapColor* i_Style);
    SurroundingBitmapsStyle* GetSurroundingBitmapsStyleByName(const Name_Z& i_Name);
    S32 GetIdSurroundingBitmapsStyle(SurroundingBitmapsStyle* i_Style);
    CyclicAnimStyle* GetCyclicAnimStyleByName(const Name_Z& i_Name);
    S32 GetCyclicAnimStyleIdByName(const Name_Z& i_Name);

    Bool AreRessourcesParsed() { return m_RessourcesParsed; }

    Bool IsDebug() { return m_Debug; }

    void SetDebug(Bool i_Debug) { m_Debug = i_Debug; }

    Bool IsHideMouse() { return m_HideMouse; }

    Dialog_G* GetDialog(S32 i_Dialog) { return m_Dialogs[i_Dialog]; }

    Dialog_G* GetDialog() { return m_Dialog; }

    void SetOldDialog(S32 i_Dialog) { m_OldDialog = i_Dialog; }

    void SetNextDialog(S32 i_Dialog) { m_NextDialog = i_Dialog; }

private:
    Bool m_UnkBool_0x12b4;
    Bool m_UnkBool_0x12b5;
    U8 m_Pad_0x12b6[2];
    S32 m_UnkS32_0x12b8;
    Bool m_UnkBool_0x12bc;
    U8 m_Pad_0x12bd[3];
    Menu3DScenery* m_Menu3DScenery;
    S32 m_UnkS32_0x12c4;
    Bool m_RessourcesParsed;
    U8 m_Unk_0x12c9[2];
    Bool m_Debug;
    Bool m_HideMouse;
    U8 m_Unk_0x12cd;
    Bool m_UnkBool_0x12ce;
    U8 m_Pad_0x12cf;
    Float m_UnkFloat_0x12d0;
    U8 m_Unk_0x12d4[12];
    Dialog_G* m_Dialogs[71];
    Dialog_G* m_Dialog;
    Color m_DialogColor;
    S32 m_OldDialog;
    S32 m_NextDialog;
    S32 m_Menu3DAnimationId;
    styleText m_StyleTexts[64];
    styleBox m_StyleBoxes[32];
    styleBitmap m_StyleBitmaps[64];
    styleBitmapColor m_StyleBitmapColors[16];
    SurroundingBitmapsStyle m_SurroundingBitmapsStyles[16];
    CyclicAnimStyle m_CyclicAnimStyles[16];
    U8 m_Unk_0xf29c[0x1c];
    Bool m_UnkBool_0xf2b8;
    U8 m_Unk_0xf2b9[0x6f];
    Bool m_UnkBool_0xf328;
    U8 m_Pad_0xf329[7];
    S32 m_CurrentVoiceId;
    Float m_VoiceTimer;
    Vec2f m_UnkVec2_0xf338;
    Bool m_UnkBool_0xf340;
    U8 m_Pad_0xf341[11];
    S32 m_ActionIds[4];
    U8 m_ActionEnabled[4];
};

#endif // _MENUMANAGER_G_H_
