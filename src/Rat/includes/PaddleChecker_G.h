#ifndef _PADDLECHECKER_G_H_
#define _PADDLECHECKER_G_H_
#include "Manipulator_Z.h"

#define PADDLE_CHECKER_PLAYER_COUNT 4

struct playerPadInfo {
    void Reset();

    S32 m_PhysicalPadId;
    Bool m_Unk_0x04;
    Float m_PressTimer;
    Bool m_NeedsPad;
    Bool m_WaitingForPad;
    U8 m_Pad_0x0e[2];
    S32 m_RequestedPadId;
    Bool m_Unk_0x14;
    U8 m_Pad_0x15[3];
};

class PaddleChecker_G : public Manipulator_Z {
public:
    PaddleChecker_G();

    virtual ~PaddleChecker_G() { }

    virtual void Init();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(94) PaddleChecker_G; }

    S32 NbOfPadRequired();
    S32 PlayerForPhysicalPad(S32 i_PhysicalPadId);
    void PlayerNotWanted(S32 i_Player);
    void SetPlayerWaitingForPad(S32 i_Player);
    S32 GetNbPadConnected();
    void SetBasicInputs();

private:
    S32 m_Unk_0x20;
    Bool m_PadPressed[PADDLE_CHECKER_PLAYER_COUNT];
    Bool m_PadDisconnected[PADDLE_CHECKER_PLAYER_COUNT];
    playerPadInfo m_PadInfos[PADDLE_CHECKER_PLAYER_COUNT];
    Float m_CheckTimer;
    U8 m_Unk_0x90;
    Bool m_Unk_0x91;
    U8 m_Pad_0x92[2];
    Float m_Unk_0x94;
    Bool m_Unk_0x98;
    U8 m_Pad_0x99[3];
};
#endif // _PADDLECHECKER_G_H_
