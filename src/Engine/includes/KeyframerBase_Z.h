#ifndef _KEYFRAMERBASE_Z_H_
#define _KEYFRAMERBASE_Z_H_

#include "Types_Z.h"

#define FL_KEYFRAMER_SMOOTH (1 << 0)   // Smooth interpolation
#define FL_KEYFRAMER_LINEAR (1 << 1)   // Linear interpolation
#define FL_KEYFRAMER_UNK_0x4 (1 << 2)  // Unknown purpose
#define FL_KEYFRAMER_SQUARE (1 << 3)   // Square interpolation
#define FL_KEYFRAMER_UNK_0x10 (1 << 4) // Unknown purpose

struct Key_Z {
public:
    Key_Z()
        : m_Time(0.0f) {
    }

    Key_Z(Float i_Time) {
        SetTime(i_Time);
    }

    inline Float GetTime() const {
        return m_Time;
    }

    inline void SetTime(Float i_Time) {
        m_Time = i_Time;
    }

protected:
    Float m_Time;
};

class Keyframer_Z {
public:
    Keyframer_Z()
        : m_Flag(FL_KEYFRAMER_SMOOTH) {
    }

public:
    U16 m_Flag; // Keyframer flags
};

#endif
