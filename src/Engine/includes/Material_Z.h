#ifndef _MATERIAL_Z_H_
#define _MATERIAL_Z_H_
#include "Math_Z.h"
#include "ResourceObject_Z.h"
#include "Bitmap_ZHdl.h"
#include "Material_ZHdl.h"
#include "Types_Z.h"
#include "MaterialFlag_Z.h"

class Material_Z;
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

    inline Vec2f& GetTranslation() {
        return m_Translation;
    }

    inline Vec2f& GetScale() {
        return m_Scale;
    }

    inline Float GetRotation() const {
        return m_Rotation;
    }

    inline Vec3f& GetDiffuse() {
        return m_DiffuseColor;
    }

    inline Vec3f& GetEmissive() {
        return m_EmissiveColor;
    }

    inline Vec4f& GetSpecular() {
        return m_Specular;
    }

    inline Float GetOpacity() const {
        return m_Opacity;
    }

    inline Bitmap_ZHdl& GetBitmap(S32 i_Id = mtl_diffuse) {
        return m_CurBmapHdl[i_Id];
    }

    inline U8 GetTileU() {
        return m_TileU;
    }

    inline U8 GetTileV() {
        return m_TileV;
    }

    inline void SetBitmap(const Bitmap_ZHdl& i_BmapHdl, S32 i_Id = mtl_diffuse) {
        m_CurBmapHdl[i_Id] = i_BmapHdl;
    }

    inline void SetTranslation(const Vec2f& i_Trans) {
        m_Translation = i_Trans;
    }

    inline void SetRotation(const Float i_Rot) {
        m_Rotation = i_Rot;
    }

    inline void SetScale(const Vec2f& i_Scale) {
        m_Scale = i_Scale;
    }

    inline void SetDiffuse(const Vec3f& i_DiffuseColor) {
        m_DiffuseColor = i_DiffuseColor;
    }

    inline void SetEmissive(const Vec3f& i_EmissiveColor) {
        m_EmissiveColor = i_EmissiveColor;
    }

    inline void SetOpacity(Float i_Opacity) {
        m_Opacity = i_Opacity;
    }

    inline void GetTileU(U8 i_TileU) {
        m_TileU = i_TileU;
    }

    inline void GetTileV(U8 i_TileV) {
        m_TileV = i_TileV;
    }

    Mat4x4 GetUVTransform4x4();

    inline Mat3x3& GetUVTransform() {
        return m_UVTransform;
    }

    // TODO: Define * operator for Mat3x3 and Vec2f
    inline void GetUVTransform(const Vec2f& i_Value, Vec2f& o_Result) {
        o_Result = m_UVTransform * i_Value;
    }

    inline Bool IsTransparent() const {
        return ((GetRenderFlag() & FL_IS_TRANSPARENT) != 0);
    }

    inline Bool IsAlphaBlended() const {
        return ((GetRenderFlag() & FL_IS_ALPHABLENDED) != 0);
    }

    inline U32 GetCollisionFlag() const {
        return m_ColFlag;
    }

    inline U32 GetRenderFlag() const {
        return m_RdrFlag;
    }

    inline void SetRenderFlag(U32 i_RenderFlag) {
        m_RdrFlag = i_RenderFlag;
    }

    inline U32 GetObjectFlag() const {
        return m_ObjectFlag;
    }

    inline void SetFlag(U8 i_Flag) {
        m_Flag = i_Flag;
    }

    inline U8 GetFlag() const {
        return m_Flag;
    }

    inline void EnableRenderFlag(U32 i_Flag) {
        m_RdrFlag |= i_Flag;
    }

    inline void DisableRenderFlag(U32 i_Flag) {
        m_RdrFlag &= ~i_Flag;
    }

    inline void EnableCollisionFlag(U32 i_Flag) {
        m_ColFlag |= i_Flag;
    }

    inline void DisableCollisionFlag(U32 i_Flag) {
        m_ColFlag &= ~i_Flag;
    }

    inline void EnableObjectFlag(U32 i_Flag) {
        m_ObjectFlag |= i_Flag;
    }

    inline void DisableObjectFlag(U32 i_Flag) {
        m_ObjectFlag &= ~i_Flag;
    }

    inline U8& GetConstantAlphaWriteValue() {
        return m_ConstantAlphaWriteValue;
    }

private:
    Vec3f m_DiffuseColor Aligned_Z(16); // $SABE: Could be wrong, maybe they had manual padding
    Float m_DiffuseOpacity;
    Vec3f m_EmissiveColor;
    Float m_Opacity; // $VIOLET: COULD BE WRONG
    Mat3x3 m_UVTransform;
    Vec4f m_Specular; // $VIOLET: Power in W
    Vec4f m_Params;   // Used by water
    Float m_Rotation;
    Vec2f m_Translation;
    Vec2f m_Scale;
    U32 m_ColFlag;
    U32 m_RdrFlag;
    U32 m_ObjectFlag;
    U8 m_Flag; // Used to indicate which bitmaps are used + pawap material
    U8 m_TileU;
    U8 m_TileV;
    U8 m_ConstantAlphaWriteValue;
    U32 m_RdrFlag2;
    WaterHeightMap_Z* m_WaterHeightMap;
    Bitmap_ZHdl m_CurBmapHdl[mtl_nb_params];
};

#endif
