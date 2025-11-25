#include "UserDefine_Z.h"
#include "Macros_Z.h"
#include "Memory_Z.h"
#include "ResourceObject_Z.h"
#include "Sys_Z.h"
#include "Types_Z.h"

UserDefine_Z::UserDefine_Z() {
    m_Cmd.m_UserDatas = NULL;
    m_Cmd.m_DataSize = 0;
}

UserDefine_Z::~UserDefine_Z() {
    if (&m_Cmd.m_UserDatas != NULL && m_Cmd.m_UserDatas) {
        Free_Z(m_Cmd.m_UserDatas);
        m_Cmd.m_DataSize = 0;
    }
}

void UserDefineCmd_Z::MakeCommandList() {
    Char* l_CurrentChar;
    S32 l_NewDataSize = 0;
    int i;
    for (i = 0; i < m_DataSize; i++) {
        if (m_UserDatas[i] == '[' || m_UserDatas[i] == ']')
            continue;
        if (m_UserDatas[i] == ';')
            l_NewDataSize++;

        l_NewDataSize++;
    }
    Char* l_NewUserData = (Char*)AllocL_Z(l_NewDataSize + 1, 137);
    int j = 0;
    for (i = 0; i < m_DataSize; i++) {
        if (m_UserDatas[i] == '[' || m_UserDatas[i] == ']')
            continue;
        if (m_UserDatas[i] == ';') {
            l_NewUserData[j++] = '\n';
            l_NewUserData[j++] = '\r';
            continue;
        }

        l_NewUserData[j++] = m_UserDatas[i];
    }
    l_NewUserData[j] = 0;
    Free_Z(m_UserDatas);
    m_UserDatas = l_NewUserData;
    m_DataSize = l_NewDataSize;
    FIXDEBUGINLINING_Z();
}

Char* UserDefineCmd_Z::GetFirstCommand(U32& o_Length) {
    m_CurLen = 0;
    return GetNextCommand(o_Length);
}

Char* UserDefineCmd_Z::GetNextCommand(U32& o_Length) {
    o_Length = 0;

    if (m_CurLen >= m_DataSize)
        return NULL;

    for (int i = m_CurLen; i < m_DataSize; i++) {
        if (m_UserDatas[i] == '\n')
            break;
        o_Length++;
    }

    m_CurLen += o_Length;
    S32 l_Pos = m_CurLen - o_Length;

    while ((m_CurLen < m_DataSize) && ((m_UserDatas[m_CurLen] == 13) || (m_UserDatas[m_CurLen] == 10) || (m_UserDatas[m_CurLen] == 32))) m_CurLen++;

    return &m_UserDatas[l_Pos];
}

void UserDefine_Z::Load(void** i_Data) {
    S32 l_DataLength;
    LOAD_Z(l_DataLength);
    Char* l_Data = (Char*)*i_Data;
    m_Cmd.SetCmd(l_Data, l_DataLength);
    *(U32*)i_Data += l_DataLength;
}
