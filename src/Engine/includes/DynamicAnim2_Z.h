#ifndef _DYNAMICANIM2_Z_H_
#define _DYNAMICANIM2_Z_H_
#include "Manipulator_Z.h"
#include "Math_Z.h"

class Node_Z;
class BoneNode_Z;
class Skel_Z;

class DynamicAnim2_Z : public Manipulator_Z {
public:
    virtual ~DynamicAnim2_Z() { }

    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    virtual void SetNode(Node_Z* i_Node) { }

    virtual void SetSkel(Skel_Z* i_Skel);
    virtual void AddConicForce(const Vec3f& a1, const Vec3f& a2, Float a3, Float a4, Float a5, Float a6, Float a7);
    virtual void AddSphericForce(const Sphere_Z& a1, Float a2, Float a3);

    virtual void SetVColGroundNormal(const Vec3f& a1) { }

    static Bool IsNodeDynamic(BoneNode_Z* i_Node, int i_Id);
    static Bool IsSkelDynamic(Skel_Z* i_Skel);

    static BaseObject_Z* NewObject() {
        return NewL_Z(146) DynamicAnim2_Z;
    }
};

#endif // _DYNAMICANIM2_Z_H_
