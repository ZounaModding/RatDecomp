#ifndef _OBJECTMOVECOLLNODE_Z_H_
#define _OBJECTMOVECOLLNODE_Z_H_

#include "Math_Z.h"
#include "Node_ZHdl.h"

class Object_Z;
class ObjectDatas_Z;
struct ColLineResult_Z;

struct ObjectMoveCollNode_Z {
public:
    Node_ZHdl m_NodeHdl;
    Vec3f m_Intersection; // World Intersection
    Vec3f m_InterGround;  // World Intersection Reception
    Vec3f m_Local;        // Object Local Intersection
    Vec3f m_LocalLookAt;  // Object Local LookAt
    Vec3f m_Front;        // Object Local Front
    S32 m_EleIdx;         // Patch ID
    Vec2f m_EleUV;        // Patch UV
    U64 m_Flag;           // Material Flag
    Float m_CollisionDistance;
    Object_Z* m_Object;
    ObjectDatas_Z* m_ObjectDatas;
    Float m_DeltaTime;

    ObjectMoveCollNode_Z() { }

    ObjectMoveCollNode_Z(ColLineResult_Z& i_Result, const Vec3f& i_LookAt) {
        // TODO: Match
    }
};

#endif // _OBJECTMOVECOLLNODE_Z_H_
