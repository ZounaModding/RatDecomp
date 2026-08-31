#ifndef _MESHCOLLISION_Z_H_
#define _MESHCOLLISION_Z_H_

#include "DynArray_Z.h"
#include "Material_ZHdl.h"
#include "SystemObject_Z.h"

class SphereCol_Z {
public:
    SphereCol_Z() {
        m_Flag = 0;
    }

    Sphere_Z m_Sphere;
    U32 m_Flag;
    Name_Z m_Name;
};

class BoxCol_Z {
public:
    BoxCol_Z() {
        m_Flag = 0;
    }

    Box_Z m_Box Aligned_Z(16);
    U32 m_Flag;
    Name_Z m_Name;
};

class CylindreCol_Z {
public:
    CylindreCol_Z() {
        m_Flag = 0;
    }

    Cylindre_Z m_Cylindre;
    U32 m_Flag;
    Name_Z m_Name;
};

class SphereColNode_Z : public SphereCol_Z {
public:
    SphereColNode_Z() {
        m_BoneNodeId = -1;
    }

    S32 m_BoneNodeId;
    U8 m_Unk_0x24[12];
};

class FaceCol_Z {
public:
    U16 m_Vertex1;
    U16 m_Vertex2;
    U16 m_Vertex3;
    U16 m_MaterialId;
};

class AABBNode_Z {
public:
    Vec3f m_Min;

    union {
        U16 m_ChildIds[2];
        AABBNode_Z* m_FirstChild;
    };

    Vec3f m_Max;

    union {
        U16 m_FaceRange[2];
        AABBNode_Z* m_SecondChild;
    };
};

class AABBCol_Z {
public:
    void Load(void** i_Data);

    void Flush() {
        m_Faces.Flush();
        m_Nodes.Flush();
    }

    DynArray_Z<FaceCol_Z, 32, FALSE, FALSE> m_Faces;
    DynArray_Z<AABBNode_Z, 32, FALSE, FALSE> m_Nodes;
};

typedef DynArray_Z<SphereCol_Z, 8> SphereCol_ZDA;
typedef DynArray_Z<BoxCol_Z, 8> BoxCol_ZDA;
typedef DynArray_Z<CylindreCol_Z, 8> CylindreCol_ZDA;

#endif
