#ifndef _PLAYANIM_Z_H_
#define _PLAYANIM_Z_H_
#include "Manipulator_Z.h"
#include "Animation_ZHdl.h"
#include "Math_Z.h"

class SequenceAnim_ZHdl;

class PlayAnim_Z : public Manipulator_Z {
public:
    virtual void Init();

    virtual ~PlayAnim_Z() { }

    virtual Bool MarkHandles();
    virtual void Reset();
    virtual void ActionOnActivate();
    virtual void ActionOnDeactivate();
    virtual void Update(Float i_DeltaTime);

    virtual void SetSequenceManager(const SequenceAnim_ZHdl& a1) { }

    virtual void AddConicForce(const Vec3f& a1, const Vec3f& a2, Float a3, Float a4, Float a5, Float a6, Float a7);
    virtual void AddSphericForce(const Sphere_Z& a1, Float a2, Float a3);

    static BaseObject_Z* NewObject() {
        return NewL_Z(152) PlayAnim_Z;
    }

    void SetAnim(const Animation_ZHdl& i_AnimHdl, S32 i_Priority = 0, Bool i_Smooth = TRUE, Float i_CurTime = 0.0f);
};

#endif // _PLAYANIM_Z_H_
