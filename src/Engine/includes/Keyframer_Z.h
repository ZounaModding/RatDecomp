#ifndef _KEYFRAMER_Z_H_
#define _KEYFRAMER_Z_H_
#include "KeyframerBase_Z.h"
#include "Math_Z.h"
#include "Types_Z.h"
#include "Handle_Z.h"

#define KEY_FLOAT_COMP_FACTOR 4096.0f
#define KEY_QUAT_COMP_FACTOR 2000.0f

struct KeyFlag_Z : public Key_Z {
public:
    KeyFlag_Z() {
    }

    KeyFlag_Z(const Float i_Time, const U32 i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    inline void Get(U32& o_Value) const {
        o_Value = m_Value;
    }

    inline U32 Get() const {
        return m_Value;
    }

    inline void Set(U32 i_Value) {
        m_Value = i_Value;
    }

private:
    U32 m_Value;
};

struct KeyHdl_Z : public Key_Z {
    friend class KeyframerHdl_Z;

public:
    KeyHdl_Z() {
    }

    KeyHdl_Z(const Float i_Time, const BaseObject_ZHdl& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    inline void Get(BaseObject_ZHdl& o_Value) const {
        o_Value = m_Value;
    }

    inline const BaseObject_ZHdl& Get() const {
        return m_Value;
    }

    inline void Set(const BaseObject_ZHdl& i_Value) {
        m_Value = i_Value;
    }

private:
    BaseObject_ZHdl m_Value;
};

struct KeyFloat_Z : public Key_Z {
public:
    KeyFloat_Z() {
    }

    KeyFloat_Z(const Float i_Time, const S16& i_Value, const Float& i_TgtIn, const Float& i_TgtOut) {
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

    inline Float GetValue() const {
        return m_Value;
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
    Float m_Value;
    Float m_TgtIn;
    Float m_TgtOut;
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
        m_Value = i_Value * KEY_FLOAT_COMP_FACTOR;
        m_TgtIn = i_TgtIn * KEY_FLOAT_COMP_FACTOR;
        m_TgtOut = i_TgtOut * KEY_FLOAT_COMP_FACTOR;
    }

    inline void Get(Float& o_Value) const {
        GetValue(o_Value);
    }

    inline Float GetValue() const {
        return m_Value * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

    inline void GetValue(Float& o_Value) const {
        o_Value = m_Value * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

    inline void GetTgtIn(Float& o_TgtIn) const {
        o_TgtIn = m_TgtIn * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

    inline void GetTgtOut(Float& o_TgtOut) const {
        o_TgtOut = m_TgtOut * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

private:
    S16 m_Value;
    S16 m_TgtIn;
    S16 m_TgtOut;
};

struct KeyFloatLinear_Z : public Key_Z {
public:
    KeyFloatLinear_Z() {
    }

    KeyFloatLinear_Z(const Float i_Time, const Float& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    void Set(const Float& i_Value) {
        m_Value = i_Value;
    }

    inline void Get(Float& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Float& o_Value) const {
        o_Value = m_Value;
    }

private:
    Float m_Value;
};

struct KeyFloatLinearComp_Z : public Key_Z {
public:
    KeyFloatLinearComp_Z() {
    }

    KeyFloatLinearComp_Z(const Float i_Time, const Float& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    void Set(const Float& i_Value) {
        m_Value = i_Value * KEY_FLOAT_COMP_FACTOR;
    }

    inline void Get(Float& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Float& o_Value) const {
        o_Value = m_Value * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

private:
    S16 m_Value;
};

struct KeyRot_Z : public Key_Z {
public:
    KeyRot_Z() {
    }

    KeyRot_Z(const Float i_Time, const Quat& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    inline void Get(Quat& o_Value) const {
        o_Value.v.x = m_Value.x * (1.0f / KEY_QUAT_COMP_FACTOR);
        o_Value.v.y = m_Value.y * (1.0f / KEY_QUAT_COMP_FACTOR);
        o_Value.v.z = m_Value.z * (1.0f / KEY_QUAT_COMP_FACTOR);
        o_Value.w = m_Value.w * (1.0f / KEY_QUAT_COMP_FACTOR);
    }

    void Set(const Quat& i_Value);

private:
    QuatComp_Z m_Value;
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

struct KeyVec3f_Z : public Key_Z {
public:
    KeyVec3f_Z() {
    }

    KeyVec3f_Z(const Float i_Time, Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut) {
        SetTime(i_Time);
        Set(i_Value, i_TgtIn, i_TgtOut);
    }

    void Set(Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut) {
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
        o_Value.x = m_ValueX * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_Value.y = m_ValueY * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_Value.z = m_ValueZ * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

    inline void GetTgtIn(Vec3f& o_TgtIn) const {
        o_TgtIn.x = m_TgtInX * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_TgtIn.y = m_TgtInY * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_TgtIn.z = m_TgtInZ * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

    inline void GetTgtOut(Vec3f& o_TgtOut) const {
        o_TgtOut.x = m_TgtOutX * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_TgtOut.y = m_TgtOutY * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_TgtOut.z = m_TgtOutZ * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

private:
    S16 m_ValueX;
    S16 m_ValueY;
    S16 m_ValueZ;
    S16 m_TgtInX;
    S16 m_TgtInY;
    S16 m_TgtInZ;
    S16 m_TgtOutX;
    S16 m_TgtOutY;
    S16 m_TgtOutZ;
};

struct KeyVec3fLinear_Z : public Key_Z {
public:
    KeyVec3fLinear_Z() {
    }

    KeyVec3fLinear_Z(const Float i_Time, Vec3f const& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    void Set(Vec3f const& i_Value) {
        m_Value = i_Value;
    }

    inline void Get(Vec3f& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Vec3f& o_Value) const {
        o_Value = m_Value;
    }

private:
    Vec3f m_Value;
};

struct KeyExtVec3f_Z : public Key_Z {
public:
    KeyExtVec3f_Z() {
    }

    KeyExtVec3f_Z(const Float i_Time, Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut) {
        SetTime(i_Time);
        Set(i_Value, i_TgtIn, i_TgtOut);
    }

    void Set(Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut) {
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

struct KeyVec2fLinear_Z : public Key_Z {
public:
    KeyVec2fLinear_Z() {
    }

    KeyVec2fLinear_Z(const Float i_Time, Vec2f const& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    void Set(Vec2f const& i_Value) {
        m_Value = i_Value;
    }

    inline const Vec2f& Get() const {
        return m_Value;
    }

    inline void Get(Vec2f& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Vec2f& o_Value) const {
        o_Value = m_Value;
    }

private:
    Vec2f m_Value;
};

struct KeyVec2fLinearComp_Z : public Key_Z {
public:
    KeyVec2fLinearComp_Z() {
    }

    KeyVec2fLinearComp_Z(const Float i_Time, Vec2f const& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    void Set(Vec2f const& i_Value) {
        m_ValueX = i_Value.x * KEY_FLOAT_COMP_FACTOR;
        m_ValueY = i_Value.y * KEY_FLOAT_COMP_FACTOR;
    }

    inline void Get(Vec2f& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Vec2f& o_Value) const {
        o_Value.x = m_ValueX * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_Value.y = m_ValueY * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

private:
    S16 m_ValueX;
    S16 m_ValueY;
};

struct KeyVec4fLinear_Z : public Key_Z {
public:
    KeyVec4fLinear_Z() {
    }

    KeyVec4fLinear_Z(const Float i_Time, Vec4f const& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    void Set(Vec4f const& i_Value) {
        m_Value = i_Value;
    }

    inline void Get(Vec4f& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Vec4f& o_Value) const {
        o_Value = m_Value;
    }

private:
    Vec4f m_Value;
};

struct KeyVec4fLinearComp_Z : public Key_Z {
public:
    KeyVec4fLinearComp_Z() {
    }

    KeyVec4fLinearComp_Z(const Float i_Time, Vec4f const& i_Value) {
        SetTime(i_Time);
        Set(i_Value);
    }

    void Set(Vec4f const& i_Value) {
        m_ValueX = i_Value.x * KEY_FLOAT_COMP_FACTOR;
        m_ValueY = i_Value.y * KEY_FLOAT_COMP_FACTOR;
        m_ValueZ = i_Value.z * KEY_FLOAT_COMP_FACTOR;
        m_ValueW = i_Value.w * KEY_FLOAT_COMP_FACTOR;
    }

    inline void Get(Vec4f& o_Value) const {
        GetValue(o_Value);
    }

    inline void GetValue(Vec4f& o_Value) const {
        o_Value.x = m_ValueX * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_Value.y = m_ValueY * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_Value.z = m_ValueZ * (1.0f / KEY_FLOAT_COMP_FACTOR);
        o_Value.w = m_ValueW * (1.0f / KEY_FLOAT_COMP_FACTOR);
    }

private:
    S16 m_ValueX;
    S16 m_ValueY;
    S16 m_ValueZ;
    S16 m_ValueW;
};

class KeyframerFlag_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyFlag_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyFlag_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyFlag_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyFlag_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, U32& o_Value, S32 i_KeyOffset = 1);
    void Load(void** i_Data);

private:
    DynArray_Z<KeyFlag_Z, 16, 0, 0> m_Keys;
};

class KeyframerHdl_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyHdl_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyHdl_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyHdl_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyHdl_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, BaseObject_ZHdl& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);
    void MarkHandles();
    void UpdateLinks();

private:
    DynArray_Z<KeyHdl_Z, 16, 0, 0> m_Keys;
};

class KeyframerFloat_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyFloat_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyFloat_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline DynArray_Z<KeyFloat_Z, 16, 0, 0>& GetKeys() {
        return m_Keys;
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyFloat_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyFloat_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Float& o_Value, Bool& o_Side, S32 i_KeyOffset = 1) const;
    S32 GetValue(Float i_Time, Float& o_Value, S32 i_KeyOffset = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Float& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyFloat_Z, 16, 0, 0> m_Keys;
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

    inline DynArray_Z<KeyFloatComp_Z, 16, 0, 0>& GetKeys() {
        return m_Keys;
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyFloatComp_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyFloatComp_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Float& o_Value, Bool& o_Side, S32 i_KeyOffset = 1) const;
    S32 GetValue(Float i_Time, Float& o_Value, S32 i_KeyOffset = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Float& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyFloatComp_Z, 16, 0, 0> m_Keys;
};

class KeyframerFloatLinear_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyFloatLinear_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyFloatLinear_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyFloatLinear_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyFloatLinear_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

    KeyframerFloatLinear_Z& operator=(const KeyframerFloatLinear_Z& i_Kfr) {
        m_Flag = i_Kfr.m_Flag;
        m_Keys = i_Kfr.m_Keys;
        return *this;
    }

private:
    DynArray_Z<KeyFloatLinear_Z, 16, 0, 0> m_Keys;
};

class KeyframerFloatLinearComp_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyFloatLinearComp_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyFloatLinearComp_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyFloatLinearComp_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyFloatLinearComp_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

    KeyframerFloatLinearComp_Z& operator=(const KeyframerFloatLinearComp_Z& i_Kfr) {
        m_Flag = i_Kfr.m_Flag;
        m_Keys = i_Kfr.m_Keys;
        return *this;
    }

private:
    DynArray_Z<KeyFloatLinearComp_Z, 16, 0, 0> m_Keys;
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

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyRot_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        FIXDEBUGINLINING_Z();
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyRot_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Quat& o_Value, S32 i_KeyOffset = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Quat& o_Value, S32 i_KeyOffset = 1) const;
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

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyBezierRot_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyBezierRot_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Quat& o_Value, S32 i_KeyOffset = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Quat& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyBezierRot_Z, 16, 0, 0> m_Keys;
};

class KeyframerVec3f_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyVec3f_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyVec3f_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec3f_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyVec3f_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyVec3f_Z, 16, 0, 0> m_Keys;
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

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec3fComp_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        FIXDEBUGINLINING_Z();
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyVec3fComp_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset = 1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_Time, Vec3f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyVec3fComp_Z, 16, 0, 0> m_Keys;
};

class KeyframerVec3fLinear_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyVec3fLinear_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyVec3fLinear_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec3fLinear_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyVec3fLinear_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

    KeyframerVec3fLinear_Z& operator=(const KeyframerVec3fLinear_Z& i_Kfr) {
        m_Flag = i_Kfr.m_Flag;
        m_Keys = i_Kfr.m_Keys;
        return *this;
    }

private:
    DynArray_Z<KeyVec3fLinear_Z, 16, 0, 0> m_Keys;
};

class KeyframerExtVec3f_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyExtVec3f_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyExtVec3f_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyExtVec3f_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyExtVec3f_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

private:
    DynArray_Z<KeyExtVec3f_Z, 16, 0, 0> m_Keys;
};

class KeyframerVec2fLinear_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyVec2fLinear_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyVec2fLinear_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec2fLinear_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyVec2fLinear_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec2f& o_Value, S32 i_KeyOffset = 1);
    void GetDerivate(Float i_Time, Vec2f& o_Value);
    void Load(void** i_Data);

    KeyframerVec2fLinear_Z& operator=(const KeyframerVec2fLinear_Z& i_Kfr) {
        m_Flag = i_Kfr.m_Flag;
        m_Keys = i_Kfr.m_Keys;
        return *this;
    }

private:
    DynArray_Z<KeyVec2fLinear_Z, 16, 0, 0> m_Keys;
};

class KeyframerVec2fLinearComp_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyVec2fLinearComp_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyVec2fLinearComp_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec2fLinearComp_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyVec2fLinearComp_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec2f& o_Value, S32 i_KeyOffset = 1);
    void Load(void** i_Data);

    KeyframerVec2fLinearComp_Z& operator=(const KeyframerVec2fLinearComp_Z& i_Kfr) {
        m_Flag = i_Kfr.m_Flag;
        m_Keys = i_Kfr.m_Keys;
        return *this;
    }

private:
    DynArray_Z<KeyVec2fLinearComp_Z, 16, 0, 0> m_Keys;
};

class KeyframerVec4fLinear_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyVec4fLinear_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyVec4fLinear_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec4fLinear_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyVec4fLinear_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec4f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

    KeyframerVec4fLinear_Z& operator=(const KeyframerVec4fLinear_Z& i_Kfr) {
        m_Flag = i_Kfr.m_Flag;
        m_Keys = i_Kfr.m_Keys;
        return *this;
    }

private:
    DynArray_Z<KeyVec4fLinear_Z, 16, 0, 0> m_Keys;
};

class KeyframerVec4fLinearComp_Z : public Keyframer_Z {
public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyVec4fLinearComp_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyVec4fLinearComp_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyVec4fLinearComp_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyVec4fLinearComp_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    S32 GetValue(Float i_Time, Vec4f& o_Value, S32 i_KeyOffset = 1) const;
    void Load(void** i_Data);

    KeyframerVec4fLinearComp_Z& operator=(const KeyframerVec4fLinearComp_Z& i_Kfr) {
        m_Flag = i_Kfr.m_Flag;
        m_Keys = i_Kfr.m_Keys;
        return *this;
    }

private:
    DynArray_Z<KeyVec4fLinearComp_Z, 16, 0, 0> m_Keys;
};

#endif // _KEYFRAMER_Z_H_
