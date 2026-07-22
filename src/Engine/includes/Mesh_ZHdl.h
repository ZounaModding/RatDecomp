#ifndef _MESH_ZHDL_H_
#define _MESH_ZHDL_H_
#include "Object_ZHdl.h"

class Mesh_Z;
class Object_Z;
HANDLE_Z(Mesh_Z, Object_Z);

class MeshData_Z;
class ObjectDatas_Z;
HANDLE_Z(MeshData_Z, ObjectDatas_Z);

typedef DynArray_Z<Mesh_ZHdl, 32> Mesh_ZHdlDA;

#endif
