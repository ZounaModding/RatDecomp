#include "GCSoundManager_Z.h"
#include <ai.h>
#include <ax.h>
#include <mix.h>
#include <os.h>
#include <math.h>

static Float Lin2LogTable[128];

static void callbackForAudioFrame() {
    BOOL l_InterruptsEnabled = OSEnableInterrupts();
    MIXUpdateSettings();
    OSRestoreInterrupts(l_InterruptsEnabled);
}

GCSoundManager_Z::GCSoundManager_Z() {
    m_MusicTrackIdx = -1;
}

GCSoundManager_Z::~GCSoundManager_Z() { }

Bool GCSoundManager_Z::Init() {
    if (!SoundManager_Z::Init()) {
        return FALSE;
    }

    Float l_LinearVolume = 0.0f;
    S32 i;
    Float* l_TableEntry = Lin2LogTable;
    for (i = 0; i < 128; i++) {
        *l_TableEntry = 150.0f * (Float)log(l_LinearVolume);
        l_LinearVolume += 1.0f / 128.0f;
        l_TableEntry++;
    }

    AIInit(NULL);
    AXInit();
    MIXInit();
    AXSetCompressor(0);
    AXRegisterCallback(callbackForAudioFrame);
    SetSfxVol(1.0f);
    SetMusicVol(1.0f);
    AXSetMode(2);
    m_SoundMode = 4;
    AXGetMaxDspCycles();

    if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
        m_SoundMode = 1;
    }
    else if (OSGetSoundMode() == OS_SOUND_MODE_STEREO) {
        m_SoundMode = 2;
    }

    return TRUE;
}

void GCSoundManager_Z::Shut() {
}

void GCSoundManager_Z::Reset() {
}

void GCSoundManager_Z::Draw(DrawInfo_Z& i_DrawInfo) {
    SoundManager_Z::Draw(i_DrawInfo);

    if (m_MusicTrackIdx) {
        return;
    }
}

void SoundManager_Z::CloseFrame() {
}

void GCSoundManager_Z::SetMuteState(U32 a1) {
}

void GCSoundManager_Z::PlaySound(Sound_ZHdl& a1, U32 a2, const Node_ZHdl& a3, BoneNode_Z* a4) {
}

void GCSoundManager_Z::CheckTrack(S32 a1) {
}

void GCSoundManager_Z::StopSound(S32 a1) {
}

void GCSoundManager_Z::Update(Float i_DeltaTime) {
}

void GCSoundManager_Z::Clean() {
}

Bool GCSoundManager_Z::Minimize() {
    return TRUE;
}

void GCSoundManager_Z::FreeSound(void* a1) {
}
