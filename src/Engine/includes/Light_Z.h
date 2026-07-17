#ifndef _LIGHT_Z_H_
#define _LIGHT_Z_H_
#include "Object_Z.h"
#include "Node_ZHdl.h"
#include "LightData_Z.h"

class Light_Z;

class Light_Z : public Object_Z {
public:
    Light_Z();

    // clang-format off
    virtual ~Light_Z() { }
    virtual void Load(void** i_Data);
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data);
    // clang-format on

    inline Vec3f& GetColor() { return m_Color; }

    inline Vec3f& GetDir() { return m_Direction; }

    inline Quat& GetRotation() { return m_Rotation; }

    inline Vec3f& GetAmbiant() { return m_AmbientColor; }

    inline Vec3f& GetTranslation() { return m_Translation; }

    inline Bool IsActive() const { return IsFlag(FL_IS_LIGHT_ACTIVE); }

    inline void SetActive(Bool i_Active = TRUE) {
        DisableFlag(FL_IS_LIGHT_ACTIVE);
        if (i_Active) {
            EnableFlag(FL_IS_LIGHT_ACTIVE);
        }
    }

    inline void SetColor(const Vec3f& i_Color) { m_Color = i_Color; }

    void SetAmbiant(const Vec3f& i_Ambient);

    inline void SetRotation(const Quat& i_Rot) { m_Rotation = i_Rot; }

    inline void SetTranslation(const Vec3f& i_Trans) { m_Translation = i_Trans; }

    inline void SetDir(const Vec3f& i_Dir) { m_Direction = i_Dir; }

    static BaseObject_Z* NewObject() { return NewL_Z(48) Light_Z; }

private:
    Node_ZHdlDA m_UserNodeHdls;
    Quat m_Rotation;
    Vec3f m_Direction;
    Vec3f m_Color;
    Float m_Intensity;
    Vec3f m_AmbientColor;
    Vec3f m_Translation;
};

#endif // _LIGHT_Z_H_
