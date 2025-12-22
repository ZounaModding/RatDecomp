#ifndef _PLAYANIM_Z_H_
#define _PLAYANIM_Z_H_
#include "Manipulator_Z.h"
#include "Animation_ZHdl.h"

class PlayAnim_Z : public Manipulator_Z {
public:
    void SetAnim(const Animation_ZHdl& i_AnimHdl, S32 i_Priority = 0, Bool i_Smooth = TRUE, Float i_CurTime = 0.0f);
};

#endif // _PLAYANIM_Z_H_
