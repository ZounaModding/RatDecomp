#ifndef _BASEOBJECT_Z_H_
#define _BASEOBJECT_Z_H_
#include "Name_Z.h"
#include "Handle_Z.h"
#include "String_Z.h"
#include "ABC_Message_Engine.h"

class ABC_Agent;

class BaseObject_Z {
public:
    Name_Z m_Name;
    BaseObject_ZHdl m_Handle;
    BaseObject_Z() { };

    virtual void Init() { };

    virtual ~BaseObject_Z() {
        m_Handle = HANDLE_NULL;
    };

    virtual void Load(void** i_Data) { };
    virtual void EndLoad() { };
    virtual void AfterEndLoad() { };
    virtual void LoadDone() { };
    virtual void LoadLinks(void** i_Data);
    virtual void EndLoadLinks();
    virtual void Clean();
    virtual Bool MarkHandles();

    void SetHandle(S32 i_ID, S8 i_Key);
    const BaseObject_ZHdl& GetHandle() const;
    String_Z<ARRAY_CHAR_MAX>* GetDebugName() const;
    String_Z<ARRAY_CHAR_MAX>* GetDebugRscName() const;
    void Send(abc_message i_Msg, ABC_Agent* i_Receiver, Float i_Param = UNDEFINED_FVALUE);
    void SendTimed(abc_message i_Msg, ABC_Agent* i_Receiver, Float i_Delay, Float i_Param = UNDEFINED_FVALUE);
    void SetName(const Name_Z& i_Name, Bool i_ResToo = FALSE);
};

#endif
