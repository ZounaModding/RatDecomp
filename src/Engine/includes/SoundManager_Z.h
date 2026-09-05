#ifndef _SOUNDMANAGER_Z_H_
#define _SOUNDMANAGER_Z_H_
#include "ClassNameResManager_Z.h"
#include "Math_Z.h"
#include "Node_ZHdl.h"
#include "ABC_AgentHdl.h"
#include "Sound_ZHdl.h"

#define SND_MGR_MAX_OCCLUDER_ZONE_IDS 7
#define SND_MGR_MAX_TRACKS 32

class BoneNode_Z;
class Node_Z;

Bool Cmd_PlayMusic();
Bool Cmd_StopMusic();
Bool Cmd_OpenSBF();
Bool Cmd_CloseSBF();
Bool Cmd_PlayDialog();

enum TrackFlags_Z {
    FL_TRACK_NONE = 0,
    FL_TRACK_UNK_0x1 = 1 << 0,
    FL_TRACK_MUTED = 1 << 1,
    FL_TRACK_UNK_0x10 = 1 << 4,
    FL_TRACK_STREAM = 1 << 9,
    FL_TRACK_FADE = 1 << 10,
    FL_TRACK_UNK_0x1000 = 1 << 12,
    FL_TRACK_DIALOG = 1 << 15,
    FL_TRACK_LINKED = 1 << 16,
    FL_TRACK_IGNORE_GLOBAL_VOLUME = 1 << 17,
    FL_TRACK_MUSIC = (1 << 12) | (1 << 11) | FL_TRACK_STREAM | (1 << 4) | (1 << 2) | (1 << 0)
};

#pragma push
#pragma pack(4)

struct Track_Z {
    Bool Release();

    S32 m_FrameRequestedNb; // Frame in which the sound was requested to start playing
    Vec4f m_RandValues;
    Sound_ZHdl m_SoundHdl;
    Node_ZHdl m_NodeHdl;
    BoneNode_Z* m_BoneNode;
    Float m_LifeTime;
    Bool m_Used;
    Float m_FadeDecrementStep; // Multiplied by dt to subtract from curfadeprogress
    Float m_CurFadeProgress;   // Goes from 1.0f to 0.0f
    S32 m_PitchRatio;
    Float m_VolumeFactor;
    ABC_AgentHdl m_AgentHdl;
    abc_message m_MsgToSend;
    U32 m_Flags;
    Float m_AttenuationStartDistance;
    Float m_AttenuationEndDistance;
    Float m_DistanceCamToSound;
    S32 m_UnkZoneId;
    S32 m_CamOccluderZoneId;
    S32 m_SoundOccluderZoneId;
    Float m_OcclusionFactor;
};

#pragma pop

// $SABE: Fake name
struct Music_Z {
    Float m_FadeOutMaxTime;
    Float m_FadeOutCountdownTimer;
    Float m_FadeInMaxTime;
    Float m_FadeInCountdownTimer;
    Float m_UnkFloat_0xc98;
    Float m_VolumeFactor;
    String_Z<ARRAY_CHAR_MAX> m_FilePath;
    U32 m_Flag;
    U32 m_UnkU32_0xda4;

    Music_Z() {
        m_FadeOutCountdownTimer = 0.0f;
        m_FadeInCountdownTimer = 0.0f;
        m_FilePath.Empty();
    }

    inline Bool UpdateFade(Float i_DeltaTime) {
        m_VolumeFactor = 1.0f;
        if (m_FadeOutCountdownTimer) {
            m_FadeOutCountdownTimer = m_FadeOutCountdownTimer - i_DeltaTime;
            if (m_FadeOutCountdownTimer < 0.0f) {
                m_FadeOutCountdownTimer = 0.0f;
                m_VolumeFactor = 0.0f;
                return TRUE;
            }
            else {
                m_VolumeFactor = 1.0f - (m_FadeOutMaxTime - m_FadeOutCountdownTimer) / m_FadeOutMaxTime;
            }
        }
        else {
            if (m_FadeInCountdownTimer) {
                m_FadeInCountdownTimer = m_FadeInCountdownTimer - i_DeltaTime;
                if (m_FadeInCountdownTimer < 0.0f) {
                    m_FadeInCountdownTimer = 0.0f;
                    m_VolumeFactor = 0.0f;
                }
                else {
                    m_VolumeFactor = (m_FadeInMaxTime - m_FadeInCountdownTimer) / m_FadeInMaxTime;
                }
            }
        }
        return FALSE;
    }
};

class SoundManager_Z : public ClassNameResManager_Z {
public:
    SoundManager_Z();

    virtual void Shut();
    virtual Bool Minimize();

    virtual ~SoundManager_Z() {
    }

    virtual Bool Init();
    virtual void CloseFrame();
    virtual void Update(Float a1);
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void UpdateTrack(Float a1, S32 a2, Float& a3, Float& a4, Vec3f& a5);

    virtual void CheckTrack(S32 a1) { }

    virtual void Reset() { }

    virtual void Clean();

    virtual void SetMuteState(U32 a1) { }

    virtual void SetSfxVol(Float a1) { }

    virtual Float GetSfxVol() { return m_SfxVolume; }

    virtual void SetDlgVol(Float a1) { }

    virtual Float GetDlgVol() { return m_DialogVolume; }

    virtual void SetMusicVol(Float a1) { }

    virtual Float GetMusicVol() { return m_MusicVolume; }

    virtual void SetRandomFreqScale(Float a1, Float a2) { }

    virtual void SetPitchRatio(Float a1, const Sound_ZHdl& a2, const Node_ZHdl& a3, BoneNode_Z* a4, S32 a5);
    virtual void SetVolumeFactor(Float i_Volume, const Sound_ZHdl& i_SoundHdl, const Node_ZHdl& i_NodeHdl, BoneNode_Z* i_BoneNode = NULL, S32 i_UnkS32 = -1);

    virtual void OpenStreamBF(const Char* a1) { }

    virtual void CloseStreamBF() { }

    virtual void LoadStreamSound(const Char* a1) { }

    virtual void PlayStreamSound(const Char* a1, U32 a2, const Node_ZHdl& a3, BoneNode_Z* a4, ABC_AgentHdl a5, int a6) { }

    virtual void StopStreamSound(const Char* a1) { }

    virtual void GetStreamSoundDuration(const Char* a1) { }

    virtual void IsStreamPlaying(const Char* a1) { }

    virtual void IsAnyStreamPlaying() { }

    virtual S32 LoadMusic(const Char* a1) {
        return 0;
    }

    virtual void PlayMusic(const Char* i_FilePath, U32 i_Flag, Float i_StopTime, Float i_FadeInTime) { }

    virtual void SkipMusic(const Name_Z& a1, const Name_Z& a2, U32 a3, Bool a4) { }

    virtual void SkipMusicQueueSize() { }

    virtual void GetMusicTime() { }

    virtual void StopMusic(Float i_FadeOutTime) { }

    virtual Bool IsMusicPlaying() { return FALSE; }

    virtual void GetLipSyncFromStream(const Node_ZHdl& a1) { }

    virtual void PlaySound(Sound_ZHdl& i_SoundHdl, U32 i_Flag, const Node_ZHdl& i_NodeHdl, BoneNode_Z* i_BoneNode = NULL);
    virtual void StopSound(Sound_ZHdl& i_SoundHdl, const Node_ZHdl& i_NodeHdl, Float i_UnkFloat);

    virtual void CreateSound(Sound_ZHdl& a1) { }

    virtual void FreeSound(void* a1) { }

    virtual void Compute3DVirtualization(S32 a1, Float& a2, Float& a3, Float& a4, Vec3f& a5);

    virtual void StopSound(S32 a1) { }

    void StopSoundOnNode(const Node_ZHdl& i_NodeHdl, Bool a2);

    virtual Float GetTrackTime(S32 i_TrackIdx) { return m_Tracks[i_TrackIdx].m_LifeTime; }

    virtual void GetTrackName(S32 i_TrackIdx, String_Z<ARRAY_CHAR_MAX>& o_Name);

    virtual Float GetDlgDTime(S32 i_TrackIdx) { return -1.0f; }

    virtual const Char* GetUsedTrackString();

    S32 GetFreeTrack();
    void FreeTrack(S32 i_TrackIdx);
    Bool IsOutOfRange(Node_Z* i_Node, BoneNode_Z* i_BoneNode);

    inline void InitFrame() {
        CloseFrame();
    }

protected:
    Track_Z m_Tracks[SND_MGR_MAX_TRACKS];
    S32 m_NbTracksFailedToPlay;
    S32 m_VpOccluderZoneIds[SND_MGR_MAX_OCCLUDER_ZONE_IDS];
    U32 m_NbFreeTracks;
    S32 m_SRamSize; // Jimmy DWARF
    S32 m_Flag;     // Jimmy DWARF
    S32 m_SoundMode;
    U32 m_MuteStateFlag;
    Float m_MusicVolume;
    Float m_MusicVolumeSecondary;
    Float m_SfxVolume;
    Float m_SfxVolumeSecondary;
    Float m_DialogVolume;
    Float m_DialogVolumeSecondary;
    Float m_RandomFreqScaleLow;
    Float m_RandomFreqScaleHigh;
    Music_Z m_Music;
};

#endif // _SOUNDMANAGER_Z_H_
