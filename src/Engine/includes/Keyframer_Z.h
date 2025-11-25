#ifndef _KEYFRAMER_Z_H_
#define _KEYFRAMER_Z_H_
#include "AnimMessage_Z.h"
#include "KeyframerBase_Z.h"
#include "Math_Z.h"
#include "ResourceObject_Z.h"
#include "Types_Z.h"

#define KEY_VEC_COMP_FACTOR 4096.0f
#define KEY_QUAT_COMP_FACTOR 2000.0f

struct KeyRot_Z : public Key_Z {
public:
    KeyRot_Z() {
    }

    KeyRot_Z(const Float i_Time, const Quat& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    inline void Get(Quat& o_Value) const {
        o_Value.v.x = m_ValueX * (1.0f / KEY_QUAT_COMP_FACTOR);
        o_Value.v.y = m_ValueY * (1.0f / KEY_QUAT_COMP_FACTOR);
        o_Value.v.z = m_ValueZ * (1.0f / KEY_QUAT_COMP_FACTOR);
        o_Value.w = m_ValueW * (1.0f / KEY_QUAT_COMP_FACTOR);
    }

    void Set(const Quat& i_Value);

private:
    S16 m_ValueX;
    S16 m_ValueY;
    S16 m_ValueZ;
    S16 m_ValueW;
};

struct KeyBezierRot_Z : public Key_Z {
public:
    KeyBezierRot_Z() {
    }

    KeyBezierRot_Z(const Float i_Time, const Vec3f& i_Value, const Vec3f& i_TgtIn, const Vec3f& i_TgtOut) {
        SetTime(i_Time);
        Set(i_Value, i_TgtIn, i_TgtOut);
    }

    inline void Set(const Vec3f& i_Value, const Vec3f& i_TgtIn, const Vec3f& i_TgtOut) {
        m_Value = i_Value;
        m_TgtIn = i_TgtIn;
        m_TgtOut = i_TgtOut;
    }

    inline void Get(Vec3f& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Vec3f& o_Value) const {
        o_Value = m_Value;
    }

    inline void GetTgtIn(Vec3f& o_TgtIn) const {
        o_TgtIn = m_TgtIn;
    }

    inline void GetTgtOut(Vec3f& o_TgtOut) const {
        o_TgtOut = m_TgtOut;
    }

private:
    Vec3f m_Value;
    Vec3f m_TgtIn;
    Vec3f m_TgtOut;
};

struct KeyFloatComp_Z : public Key_Z {
public:
    KeyFloatComp_Z() {
    }

    KeyFloatComp_Z(const Float i_Time, const S16& i_Value, const Float& i_TgtIn, const Float& i_TgtOut) {
        SetTime(i_Time);
        Set(i_Value, i_TgtIn, i_TgtOut);
    }

    inline void Set(const Float& i_Value, const Float& i_TgtIn, const Float& i_TgtOut) {
        m_Value = i_Value;
        m_TgtIn = i_TgtIn;
        m_TgtOut = i_TgtOut;
    }

    inline void Get(Float& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Float& o_Value) const {
        o_Value = m_Value;
    }

    inline void GetTgtIn(Float& o_TgtIn) const {
        o_TgtIn = m_TgtIn;
    }

    inline void GetTgtOut(Float& o_TgtOut) const {
        o_TgtOut = m_TgtOut;
    }

private:
    S16 m_Value;
    S16 m_TgtIn;
    S16 m_TgtOut;
};

struct KeyVec3fComp_Z : public Key_Z {
public:
    KeyVec3fComp_Z() {
    }

    KeyVec3fComp_Z(const Float i_Time, Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut) {
        SetTime(i_Time);
        Set(i_Value, i_TgtIn, i_TgtOut);
    }

    void Set(Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut);

    inline void Get(Vec3f& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Vec3f& o_Value) const {
        o_Value.x = m_ValueX * (1.0f / KEY_VEC_COMP_FACTOR);
        o_Value.y = m_ValueY * (1.0f / KEY_VEC_COMP_FACTOR);
        o_Value.z = m_ValueZ * (1.0f / KEY_VEC_COMP_FACTOR);
    }

    inline void GetTgtIn(Vec3f& o_TgtIn) const {
        o_TgtIn.x = m_TgtInX * (1.0f / KEY_VEC_COMP_FACTOR);
        o_TgtIn.y = m_TgtInY * (1.0f / KEY_VEC_COMP_FACTOR);
        o_TgtIn.z = m_TgtInZ * (1.0f / KEY_VEC_COMP_FACTOR);
    }

    inline void GetTgtOut(Vec3f& o_TgtOut) const {
        o_TgtOut.x = m_TgtOutX * (1.0f / KEY_VEC_COMP_FACTOR);
        o_TgtOut.y = m_TgtOutY * (1.0f / KEY_VEC_COMP_FACTOR);
        o_TgtOut.z = m_TgtOutZ * (1.0f / KEY_VEC_COMP_FACTOR);
    }

private:
    S16 m_ValueX;
    S16 m_TgtInX;
    S16 m_TgtOutX;
    S16 m_ValueY;
    S16 m_TgtInY;
    S16 m_TgtOutY;
    S16 m_ValueZ;
    S16 m_TgtInZ;
    S16 m_TgtOutZ;
};

struct KeyMessage_Z : public Key_Z {
public:
    KeyMessage_Z() {
    }

    KeyMessage_Z(const Float i_Time) {
        SetTime(i_Time);
    }

    inline U32 GetNb() const {
        return m_Messages.GetSize();
    }

    inline void SetNb(const U32 i_Nb) {
        m_Messages.SetSize(i_Nb);
    }

    inline Message_Z& Get(const S32 i_Index) {
        return m_Messages[i_Index];
    }

    inline const Message_Z& Get(const S32 i_Index) const {
        return m_Messages[i_Index];
    }

    inline void Minimize() {
        m_Messages.Minimize();
    }

private:
    Message_ZDA m_Messages;
};

class KeyframerRot_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyRot_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyRot_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(U32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyRot_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Quat& o_Value, S32 i_KeyIndex = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Quat& o_Value, S32 i_KeyIndex = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyRot_Z, 16, 0, 0> m_Keys;
};

class KeyframerBezierRot_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyBezierRot_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyBezierRot_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(U32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyBezierRot_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Quat& o_Value, S32 i_KeyIndex = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Quat& o_Value, S32 i_KeyIndex = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyBezierRot_Z, 16, 0, 0> m_Keys;
};

class KeyframerFloatComp_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyFloatComp_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyFloatComp_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(U32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyFloatComp_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Float& o_Value, Bool& o_Side, S32 i_KeyIndex = 1) const;
    S32 GetValue(Float i_Time, Float& o_Value, S32 i_KeyIndex = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Float& o_Value, S32 i_KeyIndex = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyFloatComp_Z, 16, 0, 0> m_Keys;
};

class KeyframerVec3fComp_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyVec3fComp_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyVec3fComp_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(U32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec3fComp_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyIndex = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Vec3f& o_Value, S32 i_KeyIndex = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyVec3fComp_Z, 16, 0, 0> m_Keys;
};

class KeyframerMessage_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyMessage_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyMessage_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(U32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyMessage_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 Get(Float i_StartTime, Float i_CurTime, Float i_MaxTime, const Message_Z& o_Value, RegMessage_Z& i_RegMsg);
    S32 GetValue(Float i_StartTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Value, S32& o_NbMsg, S32 i_KeyIndex = 0) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_StartTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Value, S32 i_KeyIndex = 0) const;
    void Load(void** i_Data);
    void MarkHandles();
    void EndLoad();
    void EndLinks(const ResourceObjectLink_Z& i_ResObjLink);

private:
    DynArray_Z<KeyMessage_Z, 16, 0, 0> m_Keys;
};

#endif // _KEYFRAMER_Z_H_
