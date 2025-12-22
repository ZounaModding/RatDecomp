#ifndef _ABC_MESSAGE_H_
#define _ABC_MESSAGE_H_
#include "ABC_Message_Engine.h"
#include "Handle_Z.h"

struct ABC_Message {
    BaseObject_ZHdl m_Sender;
    abc_message m_MessageID;
    ABC_Message* m_Next;
    Float m_Time;
    Float m_MessageParameter;

    ABC_Message() {
        Reset();
    }

    void Reset() {
        m_Sender = NULL;
        m_MessageID = msg_no_message;
        m_Next = NULL;
        m_Time = -1.0f;
        m_MessageParameter = UNDEFINED_FVALUE;
    }
};

#endif
