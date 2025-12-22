#ifndef _SOUND_Z_H_
#define _SOUND_Z_H_
#include "ResourceObject_Z.h"
#include "Types_Z.h"

// union SoundFlags_Z {
//     struct {
//         U8 PAUSED : 1,
//             LOOPING : 1,
//             Stereo : 1,
//             padding : 5;
//     } data;

//     U8 Flags;
// };

#define FL_SOUND_PAUSED (1 << 0)
#define FL_SOUND_LOOPING (1 << 1)
#define FL_SOUND_STEREO (1 << 2)

class Sound_Z : public ResourceObject_Z {
public:
    Sound_Z();
    void Reset();

    virtual void Init();
    virtual ~Sound_Z();
    virtual void Load(void** i_Data);

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

private:
    S32 m_Frequency;  /* 0x14 */
    S32 m_SoundSize;  /* 0x18 */
    U8 m_Flag;        /* 0x1C */
    U8 m_Unk0x1D;     /* 0x1D */
    S32 m_XRamBlock;  /* 0x20 */
    S32 m_SndDatas_Z; /* 0x24 */
};

#endif
