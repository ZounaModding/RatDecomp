#ifndef _FOG_Z_H_
#define _FOG_Z_H_
#include "Math_Z.h"
#include "Object_Z.h"
#include "Node_ZHdl.h"

class HFogData_Z;
class HFog_Z;

class HFogData_Z : public ObjectDatas_Z {

private:
    Float m_Unk0x28;
    Float m_Unk0x2C;
    Mat4x4 m_UnkMatrix;
    Vec3f m_Color;
    Float m_Active;
};

class HFog_Z : public Object_Z {
public:
    HFog_Z();

    // clang-format off
    virtual ~HFog_Z() { }                                             /* 0x08 */
    virtual void Load(void** i_Data);                                 /* 0x0C */
    virtual void Clean();                                             /* 0x10 */
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data); /* 0x14 */
    virtual void UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data); /* 0x18 */
    // clang-format on

    inline void SetWorldMatrix(const Mat4x4& i_World) {
        m_WorldMatrix = i_World;
    }

    inline Mat4x4& GetWorldMatrix() { return m_WorldMatrix; }

    inline const Mat4x4& GetWorldMatrix() const { return m_WorldMatrix; }

    inline Vec3f& GetColor() { return m_Color; }

    inline Quat& GetRotation() { return m_Rotation; }

    inline Vec3f& GetTranslation() { return m_Translation; }

    inline Vec3f& GetScale() { return m_Scale; }

    inline Bool& GetActive() { return m_Active; }

    inline Bool IsActive() const { return m_Active; }

    inline void SetActive(Bool i_Active = TRUE) { m_Active = i_Active; }

    inline void SetColor(const Vec3f& i_Color) { m_Color = i_Color; }

    inline void SetAmbiant(const Vec3f& i_Ambient);

    inline void SetRotation(const Quat& i_Rot) { m_Rotation = i_Rot; }

    inline void SetTranslation(const Vec3f& i_Trans) { m_Translation = i_Trans; }

    inline void SetScale(const Vec3f& i_Scale) { m_Scale = i_Scale; }

    static BaseObject_Z* NewObject() { return NewL_Z(57) HFog_Z; }

private:
    Mat4x4 m_WorldMatrix;
    Quat m_Rotation;
    Vec3f m_Color;
    Vec3f m_Translation;
    Vec3f m_Scale;
    Bool m_Active;
    Node_ZHdlDA m_UserNodeHdls;
};

#endif
