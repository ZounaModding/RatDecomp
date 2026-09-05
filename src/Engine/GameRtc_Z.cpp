#include "Game_Z.h"
#include "Program_Z.h"
#include "RtcAgent_Z.h"
#include "RtcPlayer_Z.h"

Bool Game_Z::TryToSuspend() {
    return !IsRtcActive(-1);
}

Bool Game_Z::IsRtcActive(S32 i_RtcIdx) {
    if (i_RtcIdx < 0) {
        for (S32 i = 0; i < m_RtcAgentHdls.GetSize(); i++) {
            if (m_RtcAgentHdls[i].IsValid()) {
                if (m_RtcAgentHdls[i]->GetPlayRtc()->IsActive()) {
                    return TRUE;
                }
            }
        }
    }
    else if (m_RtcAgentHdls[i_RtcIdx].IsValid()) {
        return m_RtcAgentHdls[i_RtcIdx]->GetPlayRtc()->IsActive();
    }

    return FALSE;
}
