#include "GCSoundManager_Z.h"
#include "Console_Z.h"

void GCSoundManager_Z::OpenStreamBF(const Char* i_FileName) {
    String_Z<256> l_FileName;
    l_FileName.StrCpy(i_FileName);
    l_FileName.StrCat(".SGC");
    if (!m_SoundBF.Open(l_FileName)) {
        Console_Z::PrintErrorString((U64)0x20000000, "Sound BigFile not found %s", i_FileName);
    }
}

void GCSoundManager_Z::CloseStreamBF() {
    StopStreamSound(NULL);
    m_SoundBF.Close();
}

void GCSoundManager_Z::LoadStreamSound(const Char* a1) {
}

void GCSoundManager_Z::PlayStreamSound(const Char* a1, U32 a2, const Node_ZHdl& a3, BoneNode_Z* a4, ABC_AgentHdl a5, int a6) {
}

void GCSoundManager_Z::StopStreamSound(const Char* i_Name) {
    U32 l_StreamType = Name_Z::GetID(".SGC");
    for (S32 i = 0; i < SND_MGR_MAX_TRACKS; i++) {
        if ((m_Tracks[i].m_Flags & FL_TRACK_STREAM) && l_StreamType == m_SndEmiters[i].m_SndStream.m_StreamType.GetID() && (!i_Name || strcmp(m_SndEmiters[i].m_SndStream.m_StreamName, i_Name) == 0)) {
            StopSound(i);
        }
    }
}

void GCSoundManager_Z::IsStreamPlaying(const Char* a1) {
}

void GCSoundManager_Z::IsAnyStreamPlaying() {
}

void GCSoundManager_Z::GetStreamSoundDuration(const Char* a1) {
}

void GCSoundManager_Z::GetLipSyncFromStream(const Node_ZHdl& a1) {
}

Float GCSoundManager_Z::GetTrackTime(S32 i_TrackIdx) {
    return m_Tracks[i_TrackIdx].m_LifeTime;
}

S32 GCSoundManager_Z::LoadMusic(const Char* a1) {
    return 0;
}

void GCSoundManager_Z::StopMusic(Float i_FadeOutTime) {
    if (i_FadeOutTime > 0.0f && IsMusicPlaying()) {
        m_Music.m_FadeOutMaxTime = i_FadeOutTime;
        m_Music.m_FadeOutCountdownTimer = i_FadeOutTime;
        m_Music.m_FilePath.Empty();
    }
    else if (m_MusicTrackIdx >= 0) {
        StopSound(m_MusicTrackIdx);
    }
}

Bool GCSoundManager_Z::IsMusicPlaying() {
    return m_MusicTrackIdx != -1;
}

void GCSoundManager_Z::GetMusicTime() {
}

void GCSoundManager_Z::PlayMusic(const Char* i_FilePath, U32 i_Flag, Float i_StopTime, Float i_FadeInTime) {
    if (gData.m_EngineFlag & FL_DISABLE_MUSIC) {
        return;
    }

    if (i_StopTime > 0.0f && IsMusicPlaying()) {
        StopMusic(i_StopTime);
        m_Music.m_FadeInMaxTime = i_FadeInTime;
        m_Music.m_FadeInCountdownTimer = i_FadeInTime;
        m_Music.m_Flag = i_Flag;
        m_Music.m_FilePath.StrCpy(i_FilePath);
        return;
    }

    S32 l_TrackIdx;
    for (l_TrackIdx = SND_MGR_MAX_TRACKS - 1; l_TrackIdx >= 0; l_TrackIdx--) {
        if (m_Tracks[l_TrackIdx].m_Used && m_Tracks[l_TrackIdx].m_Flags == FL_TRACK_STREAM) {
            U32 l_StreamType = Name_Z::GetID(".SGC");
            Bool l_IsSameStream;
            if (strcmp(m_SndEmiters[l_TrackIdx].m_SndStream.m_StreamName, i_FilePath) == 0 && m_SndEmiters[l_TrackIdx].m_SndStream.m_StreamType.GetID() == l_StreamType) {
                l_IsSameStream = TRUE;
            }
            else {
                l_IsSameStream = FALSE;
            }
            if (l_IsSameStream) {
                break;
            }
        }
    }

    if (l_TrackIdx >= 0 || (l_TrackIdx = LoadMusic(i_FilePath)) >= 0) {
        Track_Z& l_Track = m_Tracks[l_TrackIdx];
        m_SndEmiters[l_TrackIdx].m_SndStream.m_StreamFlags = i_Flag;
        l_Track.m_Flags = FL_TRACK_MUSIC;
        l_Track.m_SoundHdl = HANDLE_NULL;
        l_Track.m_NodeHdl = HANDLE_NULL;
        l_Track.m_BoneNode = NULL;
        m_MusicTrackIdx = l_TrackIdx;
    }
}
