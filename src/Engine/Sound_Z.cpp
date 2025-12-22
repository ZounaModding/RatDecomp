#include "Sound_Z.h"

Sound_Z::Sound_Z() {
    Init();
}

Sound_Z::~Sound_Z() {
    Reset();
}

void Sound_Z::Init() {
    m_SndDatas_Z = 0;
    m_Flag = 0;
    m_XRamBlock = -1;
    m_Frequency = 0;
    m_Unk0x1D = 0;
    m_SoundSize = 0;
}

void Sound_Z::Reset() {
}
