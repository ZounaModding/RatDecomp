#ifndef _OBJECTCOLLISIONTOOLS_Z_H_
#define _OBJECTCOLLISIONTOOLS_Z_H_
#include "SystemObject_Z.h"
#include "Collision_Z.h"
#include "StaticArray_Z.h"

Bool UpdateLocalCollisionSphereSA(StaticArray_Z<ColSphereResult_Z, NUM_SPHERE_HIT_MAX, FALSE>& io_Result, S32 i_PrevResultNb, Node_Z* i_Node);
void UpdateLocalCollisionSphere(ColSphereResult_Z* io_Result, S32 i_ResultNb, Node_Z* i_Node);
Bool SphereVsBox(const Sphere_Z& i_Sph, const Box_Z& i_Box);

#endif // _OBJECTCOLLISIONTOOLS_Z_H_
