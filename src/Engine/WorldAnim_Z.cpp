#include "World_Z.h"
#include "AnimFrame_Z.h"
#include "SoundManager_Z.h"
#include "Warp_Z.h"

void World_Z::UpdateAnims(Float i_DeltaTime) {
    S32 i;

    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        m_AnimFrameHdls[i]->UpdateMessage(i_DeltaTime);
    }

    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        m_AnimFrameHdls[i]->Update();
    }

    GetWarp()->UpdateAnims(i_DeltaTime);
}

Bool World_Z::StartAnimationOnNode(const Node_ZHdl& i_NodeHdl) {
    AnimFrame_ZHdl l_AnimFrameHdl = GetAnimationOnNode(i_NodeHdl, TRUE);

    if (l_AnimFrameHdl) {
        l_AnimFrameHdl->DoStart();
        return TRUE;
    }

    return FALSE;
}

AnimFrame_ZHdl World_Z::GetAnimationOnNode(const Node_ZHdl& i_NodeHdl, Bool a2) {
    for (S32 i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        if (i_NodeHdl == m_AnimFrameHdls[i]->GetAnimatedNode()) {
            return m_AnimFrameHdls[i];
        }
    }

    return HANDLE_NULL;
}

Bool World_Z::RemoveAnimation(const AnimFrame_ZHdl& i_AnimFrameHdl) {
    for (S32 i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        if (i_AnimFrameHdl == m_AnimFrameHdls[i]) {
            gData.SoundMgr->StopSoundOnNode(i_AnimFrameHdl->GetAnimatedNode(), FALSE);
            m_AnimFrameHdls.Remove(i);
            return TRUE;
        }
    }

    return FALSE;
}

void World_Z::AddAnimation(const AnimFrame_ZHdl& i_AnimFrameHdl) {
    if (!IsAnimation(i_AnimFrameHdl)) {
        m_AnimFrameHdls.Add(i_AnimFrameHdl);
    }
}

Bool World_Z::IsAnimation(const AnimFrame_ZHdl& i_AnimFrameHdl) {
    for (S32 i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        if (i_AnimFrameHdl == m_AnimFrameHdls[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

void World_Z::ResetAnimFrames() {
    for (S32 i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        m_AnimFrameHdls[i]->ResetAnim();
    }

    GetWarp()->ResetAnimFrames();
}
