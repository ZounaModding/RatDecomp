#ifndef _POINTS_Z_H_
#define _POINTS_Z_H_
#include "MeshMorph_Z.h"
#include "Object_Z.h"

typedef DynArray_Z<Vec3f, 32, 0, 0, 32> Vec3fDA;
typedef DynArray_Z<Vec2f, 32, 0, 0, 32> Vec2fDA;
typedef DynArray_Z<TBVtx, 32, 0, 0, 4> TBVtxDA;

class Points_Z : public Object_Z {
public:
    Points_Z();
    virtual ~Points_Z() { };

    static BaseObject_Z* NewObject() {
        return NewL_Z(67) Points_Z;
    }

    virtual void Load(void** i_Data);
    virtual void Clean();
    inline Vec3fDA& GetVertexDA();
    inline Vec3f& GetVertex(const U32 i_Index);
    inline void SetVertex(const U32 i_Index, const Vec3f& o_Point);
    inline U32 GetNbVertices() const;
    virtual U32 GetNbNormals() const;

protected:
    Vec3fDA m_Vertices;
    TBVtxDA m_TangentBinormalVertices;
    Morpher_Z m_Morpher;
};

#endif
