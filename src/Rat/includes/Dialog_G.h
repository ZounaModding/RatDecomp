#ifndef _DIALOG_G_H_
#define _DIALOG_G_H_
#include "Button_G.h"
#include "DynArray_Z.h"
#include "Frame_G.h"
#include "Name_Z.h"
#include "Types_Z.h"

class MenuManager_G;
class Viewport_Z;
struct DrawParam;

// $SABE: Figure these out
enum DIAL_STATE {
    DIAL_STATE_0,
    DIAL_STATE_1
};

class Dialog_G {
public:
    typedef void (Dialog_G::*MsgBoxCallback)(const void*, void*);
    typedef void (Dialog_G::*MsgBoxUpdateCallback)(const void*, void*, Float);

protected:
    DynArray_Z<Button_G> m_Buttons;

private:
    DynArray_Z<Frame_G> m_Frames;

protected:
    S32 m_State;

private:
    U8 m_Unk_0x14[44];
    S32 m_Id;

protected:
    S32 m_SelectedButtonId;

private:
    Name_Z m_NameDialog;
    Float m_PosX;
    Float m_PosY;
    Float m_SizeX;
    Float m_SizeY;
    MenuManager_G* m_MenuManager;
    U8 m_Unk_0x60[12];
    S32 m_PreviousDialog;
    MsgBoxCallback m_MsgBoxYes;
    MsgBoxCallback m_MsgBoxNo;
    MsgBoxCallback m_MsgBoxContinue;
    MsgBoxUpdateCallback m_MsgBoxUpdate;
    DynArray_Z<S32, 32, FALSE, FALSE> m_Unk_0xa0;
    DynArray_Z<S32, 32, FALSE, FALSE> m_Unk_0xa8;
    DynArray_Z<S32, 32, FALSE, FALSE> m_RandomSounds;

public:
    virtual void CheckPad2();
    virtual void Remove();
    virtual void Update(Float i_DeltaTime);
    virtual void UpdateDescription();
    virtual void UpdatePictureDescription();
    virtual void DispatchMessages();
    virtual void UpdateFocusedDialog(Float i_DeltaTime);
    virtual void UpdateDrawedDialog(Float i_DeltaTime);
    virtual void InitDialog();
    virtual void MenuActivated();
    virtual void BeforeDialog();
    virtual void AfterDialog();
    virtual void BackPressed();
    virtual void DrawDialog(const Viewport_Z* i_Viewport, DrawParam& i_DrawParam);

    Button_G* PushButton();
    Button_G* GetButton(S32 i_Id);
    void SelectButton(S32 i_Id);
    void SelectButton(Button_G* i_Button);
    void ChangeState(DIAL_STATE i_State);

    const Name_Z& GetNameDialog() { return m_NameDialog; }

    S32 GetId() { return m_Id; }

    S32 GetState() const { return m_State; }

    S32 GetSelectedButtonId() const { return m_SelectedButtonId; }

    void SetSelectedButtonId(S32 i_Id) { m_SelectedButtonId = i_Id; }

    void SetId(S32 i_Id) { m_Id = i_Id; }

    MenuManager_G* GetMenuManager() { return m_MenuManager; }

    void SetMenuManager(MenuManager_G* i_MenuManager) { m_MenuManager = i_MenuManager; }

    void SetPreviousDialog(S32 i_Dialog) { m_PreviousDialog = i_Dialog; }

    void SetPosAndSize(Float i_PosX, Float i_PosY, Float i_SizeX, Float i_SizeY) {
        m_PosX = i_PosX;
        m_PosY = i_PosY;
        m_SizeX = i_SizeX;
        m_SizeY = i_SizeY;
    }
};

#endif // _DIALOG_G_H_
