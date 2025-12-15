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
class KeyframerMessage_Z;

typedef DynArray_Z<Message_Z, 4, 0, 1> Message_ZDA;

struct Message_Z {
    S32 m_Id;
    S32 m_U32Param;
    U32 m_FlagParam;
    Float m_FloatParam;
    Name_Z m_NameParam;

    Message_Z() {
        m_Id = -1;
        m_U32Param = 0;
    }
};

struct RegMessage_Z {
    S32 m_Id;
    U32 m_U32Param;
    U32 m_FlagParam;
    Float m_FloatParam;
    Name_Z m_NameParam;
    Float m_Time;
    U8 m_UnkBytes_0x18[12];
    Skel_Z* m_SkelPtr;
    BoneNode_Z* m_BoneNodePtr;
    Node_Z* m_NodePtr;

    RegMessage_Z& operator=(const Message_Z& i_Msg) {
        m_Id = i_Msg.m_Id;
        m_U32Param = i_Msg.m_U32Param;
        m_FlagParam = i_Msg.m_FlagParam;
        m_FloatParam = i_Msg.m_FloatParam;
        m_NameParam = i_Msg.m_NameParam;
        return *this;
    }
};

struct SkelMessage_Z {
    S32 m_NextId;
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
    AnimatedAgent_Z* m_VsAgentPtr;
};

struct KeyMessage_Z : public Key_Z {
    friend class KeyframerMessage_Z;

public:
    KeyMessage_Z() { }

    KeyMessage_Z(const Float i_Time) {
        SetTime(i_Time);
    }

    ~KeyMessage_Z() { }

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

    inline DynArray_Z<Message_Z, 4, 0, 1>& GetMessages() {
        return m_Messages;
    }

    inline void Minimize() {
        m_Messages.Minimize();
    }

    inline S32 IsLinkHdl(S32 l_MsgIdx) {
        Bool l_DoMark;
        if (Get(l_MsgIdx).m_Id != -1 && Get(l_MsgIdx).m_Id >= msg_rsc_note_track_first && Get(l_MsgIdx).m_Id < msg_rsc_note_track_end) {
            l_DoMark = TRUE;
        }
        else {
            l_DoMark = FALSE;
        }
        return l_DoMark;
    }

private:
    Message_ZDA m_Messages;
};

typedef DynArray_Z<KeyMessage_Z, 32, 1, 1> KeyMessage_ZDA;

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

    inline KeyMessage_ZDA& GetKeys() {
        return m_Keys;
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyMessage_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyMessage_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        m_Keys.Flush();
    }

    S32 Get(Float i_StartTime, Float i_CurTime, Float i_MaxTime, const Message_Z& i_Msg, RegMessage_Z& o_Value);
    S32 GetValue(Float i_StartTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Value, S32& io_MsgNb, S32 i_MsgId = -1) const;
    S32 GetCctValue(S32 i_StartKey, S32 i_KeyCount, Float i_StartTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Value, S32& io_MsgNb) const;
    void Load(void** i_Data);
    void MarkHandles();
    void EndLoad();
    void EndLinks(const ResourceObjectLink_Z& i_ResObjLink);

private:
    KeyMessage_ZDA m_Keys;
};

#endif // _ANIMMESSAGE_Z_H_
