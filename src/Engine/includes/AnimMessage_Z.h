#ifndef _ANIMMESSAGE_Z_H_
#define _ANIMMESSAGE_Z_H_
#include "DynArray_Z.h"
#include "Types_Z.h"
#include "Name_Z.h"
#include "KeyframerBase_Z.h"
#include "ResourceObject_Z.h"

class Skel_Z;
class BoneNode_Z;
class Node_Z;
class AnimatedAgent_Z;
class Message_Z;

typedef DynArray_Z<Message_Z, 4, 0, 1> Message_ZDA;

struct Message_Z {
    U32 m_Id;
    U32 m_U32Param;
    U32 m_FlagParam;
    Float m_FloatParam;
    Name_Z m_NameParam;
};

struct RegMessage_Z {
    U32 m_Id;
    U32 m_U32Param;
    U32 m_FlagParam;
    Float m_FloatParam;
    Name_Z m_NameParam;
    Float m_Time;
    U8 m_UnkBytes_0x18[12];
    Skel_Z* m_SkelPtr;
    BoneNode_Z* m_BoneNodePtr;
    Node_Z* m_NodePtr;
};

struct SkelMessage_Z {
    U32 m_NextId;
    U32 m_SphereId;
    U32 m_VsSphereId;
    Bool m_CanSphereCollide;
    Bool m_CanVsSphereCollide;
    Name_Z m_SphereName;
    Name_Z m_VsSphereName;
    Name_Z m_VsName;
    Skel_Z* m_SkelPtr;
    Skel_Z* m_VsSkelPtr;
    BoneNode_Z* m_BoneNodePtr;
    BoneNode_Z* m_VsBoneNodePtr;
    AnimatedAgent_Z* m_AnimatedAgentPtr;
};

struct KeyMessage_Z : public Key_Z {
public:
    KeyMessage_Z() {
    }

    KeyMessage_Z(const Float i_Time) {
        SetTime(i_Time);
    }

    inline S32 GetNb() const {
        return m_Messages.GetSize();
    }

    inline void SetNb(const S32 i_Nb) {
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

    inline DynArray_Z<KeyMessage_Z, 16, 0, 0>& GetKeys() {
        return m_Keys;
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

    S32 Get(Float i_StartTime, Float i_CurTime, Float i_MaxTime, const Message_Z& i_Msg, RegMessage_Z& o_Value);
    S32 GetValue(Float i_StartTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Value, S32& io_KeyIndex, S32 i_MsgId) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_StartTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Value, S32& io_KeyIndex) const;
    void Load(void** i_Data);
    void MarkHandles();
    void EndLoad();
    void EndLinks(const ResourceObjectLink_Z& i_ResObjLink);

private:
    DynArray_Z<KeyMessage_Z, 16, 0, 0> m_Keys;
};

#endif // _ANIMMESSAGE_Z_H_
