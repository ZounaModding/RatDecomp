#ifndef _GCSOUNDMANAGER_Z_H_
#define _GCSOUNDMANAGER_Z_H_
#include "SoundManager_Z.h"
#include "GCSndStream_Z.h"
#include "GCSoundBF.h"

class GCSoundManager_Z : public SoundManager_Z {
public:
    GCSoundManager_Z();
    virtual void Shut();
    virtual Bool Minimize();
    virtual ~GCSoundManager_Z();
    virtual Bool Init();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(DrawInfo_Z& a1);
    virtual void CheckTrack(S32 a1);
    virtual void Reset();
    virtual void Clean();
    virtual void SetMuteState(U32 a1);
    virtual void OpenStreamBF(const Char* a1);
    virtual void CloseStreamBF();
    virtual void LoadStreamSound(const Char* a1);
    virtual void PlayStreamSound(const Char* a1, U32 a2, const Node_ZHdl& a3, BoneNode_Z* a4, ABC_AgentHdl a5, int a6);
    virtual void StopStreamSound(const Char* a1);
    virtual void GetStreamSoundDuration(const Char* a1);
    virtual void IsStreamPlaying(const Char* a1);
    virtual void IsAnyStreamPlaying();
    virtual S32 LoadMusic(const Char* a1);
    virtual void PlayMusic(const Char* a1, U32 a2, Float a3, Float a4);
    virtual void GetMusicTime();
    virtual void StopMusic(Float a1);
    virtual Bool IsMusicPlaying();
    virtual void GetLipSyncFromStream(const Node_ZHdl& a1);
    virtual void PlaySound(Sound_ZHdl& a1, U32 a2, const Node_ZHdl& a3, BoneNode_Z* a4);
    virtual void FreeSound(void* a1);
    virtual void StopSound(S32 a1);
    virtual void GetTrackTime(S32 a1);
    virtual void GetDlgDTime(S32 a1);

private:
    S32 m_MusicTrackIdx;
    SndEmiter_Z m_SndEmiters[SND_MGR_MAX_TRACKS];
    GCSoundBF_Z m_SoundBF;
};

#endif // _GCSOUNDMANAGER_Z_H_
