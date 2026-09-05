#ifndef _WARP_Z_H_
#define _WARP_Z_H_
#include "ResourceObject_Z.h"
#include "MaterialAnim_ZHdl.h"
#include "Material_ZHdl.h"
#include "Node_ZHdl.h"
#include "AnimFrame_ZHdl.h"

#define MAX_WARP_FACE 6

enum WarpFlag_Z {
    FL_WARP_FACE_BOTTOM = 1 << 0,
    FL_WARP_FACE_TOP = 1 << 1,
    FL_WARP_FACE_FRONT = 1 << 2,
    FL_WARP_FACE_LEFT = 1 << 3,
    FL_WARP_FACE_RIGHT = 1 << 4,
    FL_WARP_FACE_BACK = 1 << 5,
    FL_WARP_HIDE = 1 << 7,
    FL_WARP_HIDE_SCENE = 1 << 8,
};

// Made up class (just to get the proper virtual offset for Draw)
class WarpUser3D_Z {
public:
    virtual void Unk00();
    virtual void Unk04();
    virtual void Unk08();
    virtual void Unk0C();
    virtual void Unk10();
    virtual void Unk14();
    virtual void Unk18();
    virtual void Unk1C();
    virtual void Draw(U32 i_ViewportId);
};

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

    virtual WarpUser3D_Z* GetUser3D(U32 i_Idx) {
        return NULL;
    }

    void Update();
    void DrawScene(DrawInfo_Z& i_DrawInfo);
    void UpdateAnims(Float i_DeltaTime);
    void ResetAnimFrames();

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
