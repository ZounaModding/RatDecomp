#include "Mesh_Z.h"

Bool Mesh_Z::GetCollisionCapsule(
    Node_Z* i_Node,
    ObjectDatas_Z* i_Data,
    const Capsule_Z& i_Capsule,
    StaticArray_Z<ColSphereResult_Z, 128, FALSE, TRUE>& o_Result,
    U64 i_Flag,
    U64 i_NoFlag
) {
    return i_Data->IsFlag(FL_OBJECTDATAS_HIDE)
               ? FALSE
               : Object_Z::GetCollisionCapsule(i_Node, i_Data, i_Capsule, o_Result, i_Flag, i_NoFlag);
}
