#ifndef _DIALOG_MC_BASE_H_
#define _DIALOG_MC_BASE_H_
#include "Dialog_G.h"

class Button_G;
class MemoryCardManager_C;

class Dialog_MC_Base : public Dialog_G {
public:
    virtual void UpdateDescription();
    virtual void InitDialog();
    virtual void BeforeDialog();
    virtual void BackPressed();
    virtual void CallBack_RetryAfterMCChanged();
    virtual void CallBack_ContinueWithoutSaving();
    virtual void CallBack_ContinueWithoutAutosave();
    virtual void CallBack_RetryDetectMC();
    virtual void CallBack_LoadFailure();
    virtual S32 GetIdFirstSlot();
    virtual S32 GetIdStatus();
    virtual void CallBack_CancelDatasCreation();
    virtual void CallBack_ContinueAfterCreation();
    virtual void CallBack_FakeSave();
    virtual void CallBack_ContinueAfterAutosaveFailure();
    virtual void CallBack_CancelOverwrite();
    virtual void CallBack_ContinueAfterSave();
    virtual void CallBack_DeleteInProgress(Float i_DeltaTime);
    virtual void CallBack_Delete();
    virtual void CallBack_OKDelete();
    virtual void CallBack_CancelDelete();

protected:
    S32 m_MemoryCardState;
    S32 m_StatusButtonId;
    S32* m_TimeButtonIds;
    S32* m_CompletionButtonIds;
    S32* m_PointsButtonIds;
    Float m_MemoryCardTimer;
    S32 m_PreviousMemoryCardStatus;
    MemoryCardManager_C* m_MemoryCardManager;
    S32 m_SelectedSlot;
    S32 m_MemoryCardOperation;
    S32 m_RetryCount;
};
#endif // _DIALOG_MC_BASE_H_
