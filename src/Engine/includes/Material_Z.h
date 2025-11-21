#ifndef _MATERIAL_Z_H_
#define _MATERIAL_Z_H_
#include "Math_Z.h"
#include "ResourceObject_Z.h"
#include "Bitmap_ZHdl.h"
#include "Material_ZHdl.h"
#include "Types_Z.h"
#include "MaterialFlag_Z.h"

class WaterHeightMap_Z;

class Material_Z : public ResourceObject_Z {
public:
    enum mtl_params {
        mtl_diffuse = 0, ///< Diffuse bitmap channel
        mtl_envmap,      ///< EnvMap bitmap channel
        mtl_bumpmap,     ///< Bump Map bitmap channel
        mtl_specularmap, ///< Specular Map bitmap channel

        mtl_nb_params ///< Total number of channels supported in materials
    };

    Material_Z();
    virtual ~Material_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void Clone(Material_ZHdl& a1);
    virtual void UpdateTMatrix();
    virtual void Changed();
    static U32 DefaultRdrFlag;

private:
    U8 m_Unk0x14[0x0C];
    Vec3f m_DiffuseColor;
    Float m_DiffuseOpacity;
    Vec3f m_EmissiveColor;
    Float m_Opacity; // $VIOLET: COULD BE WRONG
    Mat3x3 m_UVTransform;
    Vec4f m_Specular; // $VIOLET: Power in W
    Vec4f m_ParamUsedByWater;
    Float m_Rotation;
    Vec2f m_Translation;
    Vec2f m_Scale;
    U32 m_ColFlag;
    U32 m_RdrFlags;
    U32 m_ObjectFlags;
    U8 m_TextureFlags;
    U8 m_TileU;
    U8 m_TileV;
    U8 m_ConstantAlphaWriteValue;
    U32 m_RdrFlags2;
    WaterHeightMap_Z* m_WaterHeightMap;
    Bitmap_ZHdl m_CurBmapHdl[mtl_nb_params];
};

#endif
