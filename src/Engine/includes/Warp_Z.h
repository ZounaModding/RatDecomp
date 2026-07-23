#ifndef _WARP_Z_H_
#define _WARP_Z_H_
#include "ResourceObject_Z.h"
#include "MaterialAnim_ZHdl.h"
#include "Material_ZHdl.h"
#include "Node_ZHdl.h"
#include "AnimFrame_ZHdl.h"

#define MAX_WARP_FACE 6

class Warp_Z : public ResourceObject_Z {
public:
    Warp_Z();

    virtual ~Warp_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void LoadDone();
    virtual Bool MarkHandles();

    virtual void Draw(DrawInfo_Z& i_DrawInfo) { }

    virtual void* GetUser3D(U32 i_Idx) {
        return NULL;
    }

protected:
    MaterialAnim_ZHdl m_MaterialAnimHdls[MAX_WARP_FACE];
    Material_ZHdl m_MaterialHdls[MAX_WARP_FACE];
    Node_ZHdl m_RootNodeHdl;
    AnimFrame_ZHdlDA m_AnimFrameHdls;
    uint m_Flag;
    Vec3f m_Vertices[8];
    Vec3f m_UnkVec3f_0xb4;
    Node_ZHdlDA m_ChildNodeHdls;
};

#endif // _WARP_Z_H_
