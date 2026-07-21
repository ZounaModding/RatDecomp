#ifndef _SOUND_Z_H_
#define _SOUND_Z_H_
#include "ResourceObject_Z.h"
#include "Types_Z.h"

#define FL_SOUND_PAUSED (1 << 0)
#define FL_SOUND_LOOPING (1 << 1)
#define FL_SOUND_STEREO (1 << 2)

class Sound_Z : public ResourceObject_Z {
public:
    Sound_Z();
    virtual void Init();
    virtual ~Sound_Z();
    virtual void Load(void** i_Data);
    void Reset();

    static BaseObject_Z* NewObject() {
        return NewL_Z(59) Sound_Z;
    }

    inline U8 GetFlag() const {
        return m_Flag;
    }

    inline Bool IsFlag(U8 i_Flag) const {
        return (m_Flag & i_Flag) != 0;
    }

    inline void EnableFlag(U8 i_Flag) {
        m_Flag |= i_Flag;
    }

    inline void DisableFlag(U8 i_Flag) {
        m_Flag &= ~i_Flag;
    }

protected:
    S32 m_Frequency;
    S32 m_SoundSize;
    U8 m_Flag;
    U8 m_Unk0x1D;
    S32 m_XRamBlock;
    void* m_Data;
};

#endif
