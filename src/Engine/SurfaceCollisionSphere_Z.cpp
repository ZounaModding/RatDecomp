#include "Surface_Z.h"

Bool Surface_Z::GetCollisionSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Seg, StaticArray_Z<ColSphereResult_Z, NUM_SPHERE_HIT_MAX, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag) {
    return FALSE;
}

Bool Surface_Z::GetCollisionTriangles(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<TriangleFlag_Z, 8, FALSE, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag) {
    return FALSE;
}
