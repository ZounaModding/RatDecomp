#include "File_Z.h"
#include "Memory_Z.h"
#include "Sys_Z.h"

File_Z::File_Z() {
    m_Buffer = NULL;
    m_Size = 0;
    m_CurPos = 0;
}

// TODO: Finish matching FileHdl_Z::~FileHdl_Z()
File_Z::~File_Z() {
    Close();
}

void File_Z::Init(S32 i_FileSize, const Char* i_Data) {
    m_Size = i_FileSize;
    m_Buffer = (Char*)AllocAlignL_Z(AlignUp_Z(m_Size, 32), 33, 32);
    if (i_Data) {
        Sys_Z::MemCpyFrom(m_Buffer, (Char*)i_Data, m_Size);
    }
    m_CurPos = 0;
}

Bool File_Z::Open(const Char* i_FilePath, U32 i_Flags) {
    if (m_Buffer != NULL) {
        return TRUE;
    }

    m_Hdl.Open(i_FilePath, i_Flags);
    if (!m_Hdl.IsOpened()) {
        return FALSE;
    }

    m_Path.StrCpy(i_FilePath);
    m_CurPos = 0;
    m_Size = 0;

    if (i_Flags & FL_FILE_WRONLY) {
        return TRUE;
    }

    if (i_Flags & FL_FILE_RDONLY) {
        m_Size = GetSize();

        if (i_Flags & FL_FILE_SKIP_READ) {
            return TRUE;
        }

        if (!m_Size) {
            Char l_Temp[512];
            sprintf(l_Temp, "%s File Size is NULL", i_FilePath);
            m_Hdl.Close();
            return FALSE;
        }

        m_Buffer = (Char*)AllocAlignL_Z(AlignUp_Z(m_Size, 32), 69, 32);
        m_Hdl.Read(m_Buffer, m_Size);
        m_Hdl.Close();
        m_CurPos = 0;

        return m_Size != 0;
    }

    return TRUE;
}

Bool File_Z::Read(void* o_Data, U32 i_Size) {
    if (!i_Size) {
        return TRUE;
    }

    Bool l_Result = TRUE;
    if (m_Hdl.IsOpened()) {
        S32 l_Read = m_Hdl.Read(o_Data, i_Size);
        l_Result = l_Read == i_Size;
    }
    else {
        if (m_CurPos >= m_Size) {
            l_Result = FALSE;
        }
        else {
            if (i_Size < 32) {
                Char* l_Data = (Char*)o_Data;
                for (S32 i = 0; i < i_Size; i++) {
                    l_Data[i] = m_Buffer[m_CurPos + i];
                }
            }
            else {
                memcpy(o_Data, &m_Buffer[m_CurPos], i_Size);
            }
        }
    }
    m_CurPos += i_Size;
    return l_Result;
}

void File_Z::Write(const void* i_Data, U32 i_Size) {
    if (i_Size) {
        m_Hdl.Write(i_Data, i_Size);
    }
}

void File_Z::Close() {
    if (m_Hdl.IsOpened()) {
        m_Hdl.Close();
    }
    if (m_Buffer) {
        Free_Z(m_Buffer);
        m_Size = 0;
        m_Buffer = NULL;
    }
}

void File_Z::Flush() {
    if (m_Hdl.IsOpened()) {
        m_Hdl.Flush();
    }
}

U32 File_Z::GetCurPos() {
    if (m_Hdl.IsOpened()) {
        return m_Hdl.GetCurPos();
    }
    return m_CurPos;
}

U32 File_Z::GetSize() {
    if (m_Hdl.IsOpened()) {
        return m_Hdl.GetSize();
    }
    return m_Size;
}

Bool File_Z::ReadString(Char* o_Str) {
    return FALSE;
}

Bool File_Z::ReadLine(Char* o_Line) {
    if (IsEOF()) {
        return FALSE;
    }

    while (!IsEOF()) {
        Char l_Char = ReadChar();
        if (l_Char != '\r') {
            if (l_Char != '\n') {
                *o_Line = l_Char;
                o_Line++;
                continue;
            }
        }
        GotoNextString();
        break;
    }
    *o_Line = '\0';
    return TRUE;
}

Char File_Z::ReadChar() {
    Char l_Char[128];
    Read(&l_Char, 1);
    return l_Char[0];
}

void File_Z::SetCurPos(U32 i_Pos) {
    m_CurPos = i_Pos;
    if (m_Hdl.IsOpened()) {
        m_Hdl.Seek(m_CurPos, 0);
    }
}

Bool File_Z::IsEOF() {
    return (U32)(m_CurPos == m_Size);
}

Bool File_Z::GotoNextString() {
    while (!IsEOF()) {
        Char l_Char = ReadChar();
        if (l_Char == ' ' || l_Char == '\r' || ((U8)(l_Char - 9) <= 1)) {
            continue;
        }
        SetCurPos(GetCurPos() - 1);
        break;
    }
    return !IsEOF();
}

Bool File_Z::DoFileExists(const Char* i_FilePath) {
    return FALSE;
}
