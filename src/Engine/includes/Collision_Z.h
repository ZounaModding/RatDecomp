#ifndef _COLLISION_Z_H_
#define _COLLISION_Z_H_
#include "SystemObject_Z.h"
class Node_Z;
class Material_Z;
class Object_Z;
class ObjectDatas_Z;

#define NUM_SPHERE_HIT_MAX 256
#define NUM_CAPSULE_HIT_MAX 128

struct ColLineResult_Z {
    Vec4f m_Intersection;
    Vec4f m_Normal;
    Float m_CollisionDistance;
    Vec2f m_EleUV;
    S32 m_EleIdx;
    U64 m_Flag;
    Material_Z* m_Material;
    Node_Z* m_Node;
    Object_Z* m_Object;
    ObjectDatas_Z* m_ObjectDatas;
    S32 m_UnkS32_0x48;
    Bool m_IsIntersect;

    ColLineResult_Z() { Reset(); }

    void Reset() {
        m_IsIntersect = FALSE;
        m_Node = NULL;
        m_Object = NULL;
        m_ObjectDatas = NULL;
        m_Flag = 0;
        m_Material = NULL;
    }
};

struct ClingLineResult_Z {
    Vec3f m_Intersection;
    Float m_CollisionDistance;
    Vec3f m_Normal;
    Float m_EdgeProgress; // Normalized position along the edge (0.0f = start, 1.0f = end)
    Node_Z* m_Node;
    Object_Z* m_Object;
    S32 m_EdgeIdx;
    Bool m_IsIntersect;

    ClingLineResult_Z() { Reset(); }

    void Reset() {
        m_IsIntersect = FALSE;
        m_Node = NULL;
        m_Object = NULL;
    }
};

struct ColSphereResult_Z {
    Vec3f m_Intersection;
    Float m_CollisionDistance;
    Vec3f m_Normal;
    Node_Z* m_Node;
    U64 m_Flag;
    Vec3f m_UnkVec3f_0x28;
    Vec3f m_UnkVec3f_0x34;

    ColSphereResult_Z() { Reset(); }

    void Reset() { m_Node = NULL; }
};

#endif
