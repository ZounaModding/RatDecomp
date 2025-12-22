#ifndef _SOUNDMANAGER_Z_H_
#define _SOUNDMANAGER_Z_H_
#include "ClassNameResManager_Z.h"
#include "Math_Z.h"
#include "Node_ZHdl.h"
#include "ABC_AgentHdl.h"
#include "Sound_ZHdl.h"

class BoneNode_Z;

class SoundManager_Z : public ClassNameResManager_Z {
public:
    SoundManager_Z();

    virtual void Shut();                                                                                                                                      /* 0x08 */
    virtual Bool Minimize();                                                                                                                                  /* 0x0C */
    virtual ~SoundManager_Z();                                                                                                                                /* 0x10 */
    virtual void Init();                                                                                                                                      /* 0x14 */
    virtual void CloseFrame();                                                                                                                                /* 0x18 */
    virtual void Update(Float a1);                                                                                                                            /* 0x1C */
    virtual void Draw(DrawInfo_Z& i_DrawInfo);                                                                                                                /* 0x20 */
    virtual void UpdateTrack(Float a1, S32 a2, Float& a3, Float& a4, Vec3f& a5);                                                                              /* 0x24 */
    virtual void CheckTrack(S32 a1);                                                                                                                          /* 0x28 */
    virtual void Reset();                                                                                                                                     /* 0x2C */
    virtual void Clean();                                                                                                                                     /* 0x30 */
    virtual void SetMuteState(U32 a1);                                                                                                                        /* 0x34 */
    virtual void SetSfxVol(Float a1);                                                                                                                         /* 0x38 */
    virtual void GetSfxVol();                                                                                                                                 /* 0x3C */
    virtual void SetDlgVol(Float a1);                                                                                                                         /* 0x40 */
    virtual void GetDlgVol();                                                                                                                                 /* 0x44 */
    virtual void SetMusicVol(Float a1);                                                                                                                       /* 0x48 */
    virtual void GetMusicVol();                                                                                                                               /* 0x4C */
    virtual void SetRandomFreqScale(Float a1, Float a2);                                                                                                      /* 0x50 */
    virtual void SetPitchRatio(Float a1, const Sound_ZHdl& a2, const Node_ZHdl& a3, BoneNode_Z* a4, S32 a5);                                                  /* 0x54 */
    virtual void SetVolumeFactor(Float i_Volume, const Sound_ZHdl& i_SoundHdl, const Node_ZHdl& i_NodeHdl, BoneNode_Z* i_BoneNode = NULL, S32 i_UnkS32 = -1); /* 0x58 */
    virtual void OpenStreamBF(const Char* a1);                                                                                                                /* 0x5C */
    virtual void CloseStreamBF();                                                                                                                             /* 0x60 */
    virtual void LoadStreamSound(const Char* a1);                                                                                                             /* 0x64 */
    virtual void PlayStreamSound(const Char* a1, U32 a2, const Node_ZHdl& a3, BoneNode_Z* a4, ABC_AgentHdl a5, int a6);                                       /* 0x68 */
    virtual void StopStreamSound(const Char* a1);                                                                                                             /* 0x6C */
    virtual void GetStreamSoundDuration(const Char* a1);                                                                                                      /* 0x70 */
    virtual void IsStreamPlaying(const Char* a1);                                                                                                             /* 0x74 */
    virtual void IsAnyStreamPlaying();                                                                                                                        /* 0x78 */
    virtual void LoadMusic(const Char* a1);                                                                                                                   /* 0x7C */
    virtual void PlayMusic(const Char* a1, U32 a2, Float a3, Float a4);                                                                                       /* 0x80 */
    virtual void SkipMusic(const Name_Z& a1, const Name_Z& a2, U32 a3, bool a4);                                                                              /* 0x84 */
    virtual void SkipMusicQueueSize();                                                                                                                        /* 0x88 */
    virtual void GetMusicTime();                                                                                                                              /* 0x8C */
    virtual void StopMusic(Float a1);                                                                                                                         /* 0x90 */
    virtual void IsMusicPlaying();                                                                                                                            /* 0x94 */
    virtual void GetLipSyncFromStream(const Node_ZHdl& a1);                                                                                                   /* 0x98 */
    virtual void PlaySound(Sound_ZHdl& i_SoundHdl, U32 i_Flag, const Node_ZHdl& i_NodeHdl, BoneNode_Z* i_BoneNode = NULL);                                    /* 0x9C */
    virtual void StopSound(Sound_ZHdl& i_SoundHdl, const Node_ZHdl& i_NodeHdl, Float i_UnkFloat);                                                             /* 0xA0 */
    virtual void CreateSound(Sound_ZHdl& a1);                                                                                                                 /* 0xA4 */
    virtual void FreeSound(void* a1);                                                                                                                         /* 0xA8 */
    virtual void Compute3DVirtualization(S32 a1, Float& a2, Float& a3, Float& a4, Vec3f& a5);                                                                 /* 0xAC */
    virtual void StopSound(S32 a1);                                                                                                                           /* 0xB0 */
    virtual void GetTrackTime(S32 a1);                                                                                                                        /* 0xB4 */
    virtual void GetDlgDTime(S32 a1);                                                                                                                         /* 0xB8 */
    virtual void GetUsedTrackString();                                                                                                                        /* 0xBC */
};

#endif // _SOUNDMANAGER_Z_H_
