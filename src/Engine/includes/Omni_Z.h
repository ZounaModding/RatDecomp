#ifndef _OMNI_Z_H_
#define _OMNI_Z_H_
#include "Object_Z.h"
#include "MaterialAnim_ZHdl.h"
#include "Material_ZHdl.h"

class Omni_Z : public Object_Z {
private:
    Mat4x4 m_TextureProjMat;
    Vec3f m_Color;
    Float m_Intensity;             // Scales the final color
    Vec3f m_ScaledColor;           // Color after intensity applied
    Float m_SpotInnerHalfAngleRad; // Inner angle for spotlight (unused in MASTER code but math is right) - Originally called InnerHalfRad
    Float m_SpotOuterHalfAngleRad; // Cutoff angle in radians for spotlight - Originally called OuterHalfRad
    Float m_SpotAtt0;              // = -1 / (cos(spotInnerHalfAngleRad) - cos(spotOuterHalfAngleRad)) - Baked from angles
    Float m_SpotAtt1;              // = 1 + (cos(spotOuterHalfAngleRad) / (cos(spotInnerHalfAngleRad) - cos(spotOuterHalfAngleRad))) - Baked from angles
    Float m_Start;                 // Start of falloff (linear interp on pc at least)
    Float m_End;                   // End of falloff (linear interp on pc at least)
    MaterialAnim_ZHdl m_MaterialAnimHdl;
    Material_ZHdl m_MaterialHdl;

public:
    Omni_Z();

    virtual ~Omni_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data);

    void Changed();
    // TODO: Define OmniFrust_Z
    //static void InFrustrum(DrawInfo_Z& i_DrawInfo, const Sphere_Z& i_UnkSph, Node_Z* i_Node, Omni_Z* i_Omni, OmniFrust_Z& i_OmniFrust, U8 i_Index);
    static U32 SetOmnis(const Box_Z& i_UnkBox, DrawInfo_Z& i_DrawInfo, Bool i_LocalValue, Bool i_UnkBool, U32 i_UnkU32_1, U32 i_UnkU32_2);
    static U32 SubOmnis(const Sphere_Z& i_UnkSph, const Box_Z& i_UnkBox, DrawInfo_Z& i_DrawInfo, U32 i_UnkU32_1, U32 i_UnkU32_2);

    inline void SetColor(const Vec3f& i_Color) {
        m_Color = i_Color;
        Changed();
    }

    inline const Vec3f& GetColor() const {
        return m_ScaledColor;
    }

    inline void SetIntensity(Float i_Intensity) {
        m_Intensity = i_Intensity;
        Changed();
    }

    inline Float GetIntensity() const {
        return m_Intensity;
    }

    void SetSpot(Float i_InnerHalfAngleDeg, Float i_OuterHalfAngleDeg);

    inline Float GetSpotAtt0() const {
        return m_SpotAtt0;
    }

    inline Float GetSpotAtt1() const {
        return m_SpotAtt1;
    }

    inline void SetStart(Float i_Start) { m_Start = i_Start; }

    inline Float GetStart() const { return m_Start; }

    void SetEnd(Float i_End);

    inline Float GetEnd() const { return m_End; }

    inline void SetMaterial(const Material_ZHdl& i_Material) {
        m_MaterialHdl = i_Material;
    }

    inline const Material_ZHdl& GetMaterial() const {
        return m_MaterialHdl;
    }

    void SetMaterialAnim(const MaterialAnim_ZHdl& i_MaterialAnim);

    inline const MaterialAnim_ZHdl& GetMaterialAnim() const {
        return m_MaterialAnimHdl;
    }
};

#endif // _OMNI_Z_H_
