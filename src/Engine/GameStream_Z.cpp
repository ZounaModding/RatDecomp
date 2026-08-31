#include "Game_Z.h"

// TODO: Finish matching
void Game_Z::Stream(const Vec3f& i_Pos, Agent_ZHdl i_NotifyAgent, abc_message i_Msg) {
    if (i_NotifyAgent.IsValid()) {
        m_StreamNotifyAgent = i_NotifyAgent;
        m_StreamNotifyMsg = i_Msg;
        m_StreamWorldPos = i_Pos;
    }

    Vec3f l_Pos;
    if (m_StreamNotifyAgent.IsValid()) {
        l_Pos = m_StreamWorldPos;
    }
    else {
        l_Pos = i_Pos;
    }

    S32DA l_UnkSubLevelIdDA1;
    S32DA l_UnkSubLevelIdDA2;

    // TODO: Implement the rest
}
