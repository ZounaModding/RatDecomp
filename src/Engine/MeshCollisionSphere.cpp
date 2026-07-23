#include "Mesh_Z.h"

Bool Mesh_Z::GetCollisionSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Seg, StaticArray_Z<ColSphereResult_Z, 256, 0, 1>& o_Result, U64 i_Flag, U64 i_NoFlag) {
    return FALSE;
}

Bool Mesh_Z::GetCollisionBoxes(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<BoxFlag_Z, 8, FALSE, FALSE, 4>& o_Result, U64 i_Flag, U64 i_NoFlag) {
    return FALSE;
}

Bool Mesh_Z::GetCollisionSpheres(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<SphereFlag_Z, 8, FALSE, FALSE, 4>& o_Result, U64 i_Flag, U64 i_NoFlag) {
    return FALSE;
}
