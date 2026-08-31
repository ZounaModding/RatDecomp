#include "SoundManager_Z.h"
#include "Console_Z.h"
#include "ABC_ScriptManager.h"
#include "DebugTools_Z.h"
#include "Occluder_Z.h"
#include <stdlib.h>

static Char SndMgrStatusString[ARRAY_CHAR_MAX];

SoundManager_Z::SoundManager_Z() {
    REGISTERCOMMANDC("PlayMUsic", Cmd_PlayMusic, " MusicName");
    REGISTERCOMMANDC("PlayDIalog", Cmd_PlayDialog, " Id");
    REGISTERCOMMANDC("StopMUsic", Cmd_StopMusic, " MusicName");
    REGISTERCOMMANDC("OpenSBF", Cmd_OpenSBF, " BigFileName");
    REGISTERCOMMAND("CloseSBF", Cmd_CloseSBF);

    for (S32 i = 0; i < SND_MGR_MAX_TRACKS; i++) {
        FreeTrack(i);
        m_Tracks[i].m_RandValues = Vec4f(rand() / 65536.0f, rand() / 65536.0f, rand() / 65536.0f, 1.0f);
    }

    m_NbFreeTracks = 0;
    m_MuteStateFlag = 0;
    m_RandomFreqScaleLow = 0.05f;
    m_RandomFreqScaleHigh = 0.0f;
    m_MusicVolume = 1.0f;
    m_MusicVolumeSecondary = 1.0f;
    m_SfxVolume = 1.0f;
    m_SfxVolumeSecondary = 1.0f;
    m_DialogVolume = 1.0f;
    m_DialogVolumeSecondary = 1.0f;
    m_SoundMode = 2;
    m_NbTracksFailedToPlay = 0;
}

void SoundManager_Z::Shut() {
}

Bool SoundManager_Z::Init() {
    return TRUE;
}

Bool SoundManager_Z::Minimize() {
    return TRUE;
}

void SoundManager_Z::Clean() {
}

S32 SoundManager_Z::GetFreeTrack() {
    S32 l_FreeTrack = -1;
    Float l_MaxLifeTime = 0.0f;

    for (S32 i = m_NbFreeTracks; i < SND_MGR_MAX_TRACKS; i++) {
        Track_Z& l_Track = m_Tracks[i];
        if (!l_Track.m_Used) {
            return i;
        }
        if (!(l_Track.m_Flags & FL_TRACK_STREAM) && !(l_Track.m_Flags & FL_TRACK_UNK_0x10) && !(l_Track.m_Flags & FL_TRACK_LINKED) && l_Track.m_LifeTime > l_MaxLifeTime) {
            l_MaxLifeTime = l_Track.m_LifeTime;
            l_FreeTrack = i;
        }
    }

    if (l_FreeTrack >= 0) {
        StopSound(l_FreeTrack);
        FreeTrack(l_FreeTrack);
    }
    return l_FreeTrack;
}

void SoundManager_Z::FreeTrack(S32 i_TrackIdx) {
    if (i_TrackIdx < 0) {
        return;
    }

    m_Tracks[i_TrackIdx].Release();
}

Bool Track_Z::Release() {
    Bool l_Used = m_Used;

    if (m_AgentHdl.IsValid()) {
        gData.ScriptMgr->Send(m_MsgToSend, m_AgentHdl);
    }

    m_Flags = FL_TRACK_NONE;
    m_BoneNode = NULL;
    m_Used = FALSE;
    m_LifeTime = 0.0f;
    m_DistanceCamToSound = 0.0f;
    m_SoundHdl = HANDLE_NULL;
    m_NodeHdl = HANDLE_NULL;
    m_AgentHdl = HANDLE_NULL;
    m_FrameRequestedNb = gData.m_FrameCount;
    m_UnkZoneId = -1;
    m_CamOccluderZoneId = -1;
    m_SoundOccluderZoneId = -1;
    m_OcclusionFactor = 1.0f;

    return l_Used;
}

void SoundManager_Z::SetPitchRatio(Float a1, const Sound_ZHdl& a2, const Node_ZHdl& a3, BoneNode_Z* a4, S32 a5) {
}

void SoundManager_Z::SetVolumeFactor(Float i_Volume, const Sound_ZHdl& i_SoundHdl, const Node_ZHdl& i_NodeHdl, BoneNode_Z* i_BoneNode, S32 i_UnkS32) {
}

void SoundManager_Z::PlaySound(Sound_ZHdl& i_SoundHdl, U32 i_Flag, const Node_ZHdl& i_NodeHdl, BoneNode_Z* i_BoneNode) {
}

void SoundManager_Z::StopSound(Sound_ZHdl& i_SoundHdl, const Node_ZHdl& i_NodeHdl, Float i_UnkFloat) {
}

void SoundManager_Z::UpdateTrack(Float i_DeltaTime, S32 i_TrackIdx, Float& o_Volume, Float& o_DistanceOnCameraX, Vec3f& o_Position) {
    Track_Z& l_Track = m_Tracks[i_TrackIdx];

    if ((l_Track.m_Flags & FL_TRACK_DIALOG) && (l_Track.m_Flags & FL_TRACK_STREAM)) {
        o_Volume = m_DialogVolume * l_Track.m_VolumeFactor;
    }
    else {
        o_Volume = m_SfxVolume * l_Track.m_VolumeFactor;
    }

    if (l_Track.m_Flags & FL_TRACK_IGNORE_GLOBAL_VOLUME) {
        o_Volume = l_Track.m_VolumeFactor;
    }

    o_DistanceOnCameraX = 0.0f;
    if (l_Track.m_Flags & FL_TRACK_MUTED) {
        o_Volume = 0.0f;
    }
    else if (l_Track.m_NodeHdl.IsValid()) {
        if (!(l_Track.m_Flags & FL_TRACK_UNK_0x1000) && IsOutOfRange(l_Track.m_NodeHdl, l_Track.m_BoneNode)) {
            StopSound(i_TrackIdx);
        }
        else {
            Float l_DistanceCamToSound;
            Float l_Attenuation;
            Compute3DVirtualization(i_TrackIdx, l_Attenuation, o_DistanceOnCameraX, l_DistanceCamToSound, o_Position);
            o_Volume *= l_Attenuation;
        }
    }

    if (l_Track.m_Flags & FL_TRACK_FADE) {
        o_Volume *= l_Track.m_CurFadeProgress;
        l_Track.m_CurFadeProgress -= l_Track.m_FadeDecrementStep * i_DeltaTime;
        if (l_Track.m_CurFadeProgress < 0.0f) {
            StopSound(i_TrackIdx);
        }
    }
}

void SoundManager_Z::Compute3DVirtualization(S32 a1, Float& a2, Float& a3, Float& a4, Vec3f& a5) {
}

void SoundManager_Z::Update(Float i_DeltaTime) {
    if (m_Music.UpdateFade(i_DeltaTime)) {
        StopMusic(0.0f);
        if (m_Music.m_FilePath.StrLen()) {
            PlayMusic(m_Music.m_FilePath, m_Music.m_Flag, 0.0f, 0.0f);
        }
    }

    S32 l_VpNb = gData.MainRdr->GetNbViewport();
    S32 i;
    for (i = 0; i < l_VpNb; i++) {
        Occluder_Z* l_Occluder = gData.MainRdr->GetViewport(i).GetOccluder();
        if (l_Occluder) {
            l_Occluder->GetZone(gData.MainRdr->GetViewport(i).GetCameraPos(), m_VpOccluderZoneIds[i]);
        }
        else {
            m_VpOccluderZoneIds[i] = -1;
        }
    }
    while (i < SND_MGR_MAX_OCCLUDER_ZONE_IDS) {
        m_VpOccluderZoneIds[i] = -1;
        i++;
    }
}

void SoundManager_Z::Draw(DrawInfo_Z& i_DrawInfo) {
    if (!(gData.m_EngineFlag & FL_DEBUG_SOUND)) {
        return;
    }

    Vec2f l_Position(16.0f, 160.0f);
    String_Z<ARRAY_CHAR_MAX> l_Text;
    String_Z<ARRAY_CHAR_MAX> l_TrackName;
    Color l_Color(1.0f, 1.0f, 1.0f, 1.0f);

    l_Text.SprintfCat(
        "FxVolume=%1.2f, DlgVolume=%1.2f, MusicVolume=%1.2f",
        gData.SoundMgr->GetSfxVol(),
        gData.SoundMgr->GetDlgVol(),
        gData.SoundMgr->GetMusicVol()
    );
    gData.MainRdr->DrawString(l_Position, l_Text, l_Color, 0.1f, 1.0f);

    l_Position.y += 10.0f;
    for (U32 i = 0; i < SND_MGR_MAX_TRACKS; i++) {
        Track_Z& l_Track = m_Tracks[i];
        l_Text.Empty();

        if (l_Track.m_Used) {
            Float l_TimeAfterDialog;
            Float l_TrackTime = GetTrackTime(i);
            Float l_DlgDTime = GetDlgDTime(i);
            l_TimeAfterDialog = 0.0f;
            if (l_DlgDTime >= l_TimeAfterDialog) {
                l_TimeAfterDialog = l_TrackTime - GetDlgDTime(i);
            }

            l_Color = Color(1.0f, 1.0f, 1.0f, 1.0f);
            l_Text.Sprintf("%2d ", i);
            if (l_Track.m_Flags & FL_TRACK_MUTED) {
                l_Text.StrCat("MUTED ");
                l_Color = Color(0.3f, 0.3f, 0.3f, 1.0f);
            }
            if (l_Track.m_Flags & FL_TRACK_LINKED) {
                l_Text.StrCat("LINKED ");
            }
            l_Text.SprintfCat(
                "%3dV %8.3fs %6.3fd %5.1fm ",
                (S32)(100.0f * l_Track.m_VolumeFactor),
                l_TrackTime,
                l_TimeAfterDialog,
                l_Track.m_DistanceCamToSound
            );
            GetTrackName(i, l_TrackName);
            l_Text.StrCat(l_TrackName);

            if ((l_Track.m_Flags & FL_TRACK_STREAM) && !(l_Track.m_Flags & 1)) {
                l_Color = Color(0.8f, 0.2f, 0.2f, 1.0f);
            }
        }
        else {
            l_Color = Color(0.3f, 0.3f, 0.3f, 1.0f);
            l_Text.Sprintf("%2d - FREE", i);
        }

        gData.MainRdr->DrawString(l_Position, l_Text, l_Color, 0.1f, 1.0f);
        l_Position.y += 10.0f;
    }
}

void SoundManager_Z::GetTrackName(S32 i_TrackIdx, String_Z<ARRAY_CHAR_MAX>& o_Name) {
    o_Name.Empty();
    if (m_Tracks[i_TrackIdx].m_Used) {
    }
}

Bool Cmd_PlayMusic() {
    return TRUE;
}

Bool Cmd_StopMusic() {
    return TRUE;
}

Bool Cmd_OpenSBF() {
    return TRUE;
}

Bool Cmd_CloseSBF() {
    return TRUE;
}

Bool Cmd_PlayDialog() {
    return TRUE;
}

const Char* SoundManager_Z::GetUsedTrackString() {
    return "TODO: Implement GetUsedTrackString";
}
