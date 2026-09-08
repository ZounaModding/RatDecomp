#ifndef _LEVELAGENT_G_H_
#define _LEVELAGENT_G_H_
#include "Agent_Z.h"
#include "LevelManipulator_GHdl.h"
#include "Math_Z.h"
#include "Name_Z.h"

#define LEVEL_AGENT_COMMAND_BUFFER_COUNT 8
#define LEVEL_AGENT_COMMAND_BUFFER_SIZE 256

struct TextVariable {
    Name_Z m_Name;
    Char m_Value[64];
};

typedef DynArray_Z<TextVariable, 1> TextVariableDA;

// clang-format off

BEGIN_AGENT_CLASS(LevelAgent_G, Agent_Z, 28)
public:
    LevelAgent_G() {}

    virtual ~LevelAgent_G() {}
    virtual void Init();

    DECL_BHV(BhvToLevel);
    DECL_BHV(BhvChangeLevel);
    DECL_BHV(BhvToMenu);

    inline Char* GetAfterMenuBuffer(S32 i_Index) { return m_CommandBuffers[i_Index]; }

    inline void RemoveNeedToPlacePlayer() { m_NeedToPlacePlayer = FALSE; }

private:
    Vec3f m_SavedPlayerPos;
    S32 m_Unk_0x6c;
    Quat m_SavedPlayerRot;
    S32 m_SavedWorld;
    Float m_TimeInLevel;
    Bool m_Unk_0x88;
    Char m_LaunchRtc[16];
    Char m_LaunchRtcParam[32];
    Bool m_Unk_0xb9;
    Bool m_HasNextLevelRtc;
    Bool m_FadeFromBlackOnRtcShut;
    Float m_Unk_0xbc;
    Float m_Unk_0xc0;
    Float m_Unk_0xc4;
    Char m_PlayMovie[16];
    Char m_NextLevelRtc[16];
    S32 m_NextLevel;
    U8 m_Unk_0xec[16];
    S32 m_PendingRtcLevel;
    Bool m_NeedToPlacePlayer;
    Bool m_NeedFadeFromBlack;
    Bool m_Unk_0x102;
    Bool m_Unk_0x103;
    Char m_CommandBuffers[LEVEL_AGENT_COMMAND_BUFFER_COUNT][LEVEL_AGENT_COMMAND_BUFFER_SIZE];
    LevelManipulator_GHdl m_LevelManipulatorHdl;
    TextVariableDA m_TextVariables;
END_AGENT_CLASS

// clang-format on
#endif // _LEVELAGENT_G_H_
