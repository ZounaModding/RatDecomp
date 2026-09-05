#include "ConsoleInterp_Z.h"
#include "Program_Z.h"
#include "Console_Z.h"
#include "StreamManager_Z.h"
#include <stdlib.h>

void ConsoleInterp_Z::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_notpaused_third);
    m_StackCommNb = 0;
}

void ConsoleInterp_Z::Start(const Char* i_CommandLine, S32 i_FileSize) {
    S32 l_Idx = m_FileStack.Add();
    FileInterp_Z& l_FileInterp = m_FileStack[l_Idx];
    l_FileInterp.m_LinesOrCommandsParsed = 0;
    l_FileInterp.m_CommentCurrentDepth = 0;
    l_FileInterp.m_StackIndex = m_StackCommNb;
    l_FileInterp.m_ParamCount = 0;
    l_FileInterp.m_Flags = FL_CONSOLE_UNK_0x10;
    l_FileInterp.m_File.Init(i_FileSize, i_CommandLine);
    Activate();
}

void ConsoleInterp_Z::Start(const Char* i_FileName, Char** i_CommandLine, S32 i_ParamCount) {
    U32 l_ConsFlags = gData.Cons->IsFlagEnable(FL_CONSOLE_UNK_0x10);
    gData.Cons->DisableFlag(FL_CONSOLE_UNK_0x10);
    S32 l_Idx = m_FileStack.Add();
    FileInterp_Z& l_FileInterp = m_FileStack[l_Idx];
    l_FileInterp.m_LinesOrCommandsParsed = 0;
    l_FileInterp.m_CommentCurrentDepth = 0;
    l_FileInterp.m_StackIndex = m_StackCommNb;
    l_FileInterp.m_ParamCount = i_ParamCount - 2; // The first two parameters will be BSource/Source and the script name
    l_FileInterp.m_Flags = l_ConsFlags;
    l_FileInterp.m_FileName.StrCpy(i_FileName);
    for (S32 i = 0; i < i_ParamCount - 2; i++) {
        l_FileInterp.m_Params[i].StrCpy(i_CommandLine[i + 2]);
    }
    gData.StreamMgr->Open(i_FileName, &l_FileInterp, NULL, 0, (S32)this);
    Deactivate();
}

void FileInterp_Z::Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined) {
    m_File.Init(i_FileSize, NULL);
    gData.StreamMgr->Read(m_File.GetBuf(), 0, i_FileSize, this, i_UserDefined);
}

void FileInterp_Z::Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined) {
    ((Manipulator_Z*)i_UserDefined)->Activate();
    gData.StreamMgr->Close(this, TRUE);
}

// TODO: Finish matching
void ConsoleInterp_Z::Update(Float i_DeltaTime) {
    if (m_TimeSpentRunning == 0.0f) {
        i_DeltaTime = 0.0f;
    }
    U32 l_StackSize;
    U32 l_Flags = FL_CONSOLE_NONE;
    l_StackSize = m_FileStack.GetSize();
    if (l_StackSize) {
        l_Flags = m_FileStack[l_StackSize - 1].m_Flags;
    }
    String_Z<CONSOLE_STATIC_COMMAND_LEN_MAX> l_CommandString;
    String_Z<CONSOLE_STACK_COMMAND_LEN_MAX> l_CommandExecute;
    if (!(l_Flags & FL_CONSOLE_UNK_0x10)) {
        if (gData.Cons->IsFlagEnable(FL_CONSOLE_PAUSED)) {
            return;
        }
        if (PopCommand(l_CommandExecute)) {
            Bool l_Skipped;
            S32 l_NbAdvanced = 0;
            DoCommand(1, l_CommandExecute, l_NbAdvanced, l_Skipped);
            End();
            return;
        }
    }
    if (!gData.Cons->IsFlagEnable(FL_CONSOLE_UNK_0x1)) {
        if (!gData.Cons->IsFlagEnable(FL_CONSOLE_UNK_0x2)) {
            U32 l_LastIdx = m_FileStack.GetSize() - 1;
            for (;;) {
                Bool l_Skipped;
                l_CommandString.Empty();
                if (m_FileStack[l_LastIdx].m_File.ReadLine(l_CommandString)) {
                    S32 l_UnusedLength = l_CommandString.StrLen();
                    DoCommand(
                        2,
                        l_CommandString,
                        m_FileStack[l_LastIdx].m_CommentCurrentDepth,
                        l_Skipped,
                        &m_FileStack[l_LastIdx]
                    );
                    m_FileStack[l_LastIdx].m_LinesOrCommandsParsed++;
                    if (!m_IsActive) {
                        return;
                    }
                    if (!(m_FileStack[l_LastIdx].m_Flags & FL_CONSOLE_UNK_0x10 || !gData.Cons->IsFlagEnable(FL_CONSOLE_PAUSED | FL_CONSOLE_UNK_0x1))) {
                        return;
                    }
                    if (gData.Cons->IsFlagEnable(FL_CONSOLE_PAUSED | FL_CONSOLE_UNK_0x1)) {
                        m_FileStack[l_LastIdx].m_Flags &= ~FL_CONSOLE_UNK_0x10;
                        break;
                    }
                    if (l_Skipped) {
                        continue;
                    }
                }
                if (m_FileStack[l_LastIdx].m_File.IsEOF()) {
                    break;
                }
                if (i_DeltaTime && !(m_FileStack[l_LastIdx].m_Flags & FL_CONSOLE_UNK_0x10)) {
                    break;
                }
            }
            if (m_FileStack[l_LastIdx].m_File.IsEOF()) {
                m_FileStack.Remove(l_LastIdx);
                m_FileStack.Minimize();
            }
        }
        else {
            gData.Cons->DisableFlag(FL_CONSOLE_UNK_0x2);
        }
    }
    End();
}

// TODO: Finish matching
Bool ConsoleInterp_Z::DoCommand(U32 i_Depth, Char* i_CommandString, S32& io_CommentCurrentDepth, Bool& o_Skipped, FileInterp_Z* i_ReplacementFileInterp) {
    o_Skipped = TRUE;
    S32 l_StrLen = strlen(i_CommandString);
    if (l_StrLen) {
        Char* l_BeginComment = strstr(i_CommandString, "/*");
        Char* l_EndMultiLineComment = strstr(i_CommandString, "*/");
        while (l_BeginComment || l_EndMultiLineComment) {
            if (l_BeginComment && l_EndMultiLineComment) {
                l_StrLen = strlen(l_EndMultiLineComment);
                strncpy(l_BeginComment, l_EndMultiLineComment + 2, l_StrLen - 1);
            }
            else if (l_BeginComment) {
                if (io_CommentCurrentDepth) {
                    *i_CommandString = '\0';
                }
                else {
                    *l_BeginComment = '\0';
                }
                io_CommentCurrentDepth++;
            }
            else {
                l_StrLen = strlen(l_EndMultiLineComment);
                strncpy(i_CommandString, l_EndMultiLineComment + 2, l_StrLen - 1);
                io_CommentCurrentDepth--;
            }
            l_BeginComment = strstr(i_CommandString, "/*");
            l_EndMultiLineComment = strstr(i_CommandString, "*/");
        }
        Char* l_Comment = strstr(i_CommandString, "//");
        if (l_Comment) {
            if (l_Comment == i_CommandString) {
                return TRUE;
            }
            *l_Comment = '\0';
        }
        U32 l_StrLen = strlen(i_CommandString);
        if (!l_StrLen || io_CommentCurrentDepth) {
            return TRUE;
        }
        o_Skipped = FALSE;
        if (i_ReplacementFileInterp) {
            String_Z<CONSOLE_STATIC_COMMAND_LEN_MAX> l_ReplacedCommandString;
            l_ReplacedCommandString.Empty();
            if (i_ReplacementFileInterp->ReplaceParams(i_CommandString, l_ReplacedCommandString)) {
                return gData.Cons->NewCommand(l_ReplacedCommandString, i_Depth);
            }
        }
        if (!gData.Cons->NewCommand(i_CommandString, i_Depth)) {
            return FALSE;
        }
    }
    return TRUE;
}

Bool ConsoleInterp_Z::PushCommand(const Char* i_CommandLine, Bool i_TopOfStack) {
    ASSERTLE_Z(strlen(i_CommandLine) < CONSOLE_STACK_COMMAND_LEN_MAX, "", 244, "strlen(CommandLine)<80");
    ASSERTLE_Z(m_StackCommNb != CONSOLE_MAX_STACK_COMMAND, "", 245, "StackCommNb!=2048");

    if (m_StackCommNb == CONSOLE_MAX_STACK_COMMAND) {
        return FALSE;
    }

    if (i_TopOfStack) {
        strcpy(m_StackComm[m_StackCommNb].m_CommandLine, i_CommandLine);
        m_StackComm[m_StackCommNb].m_CommandName = Name_Z::GetID(i_CommandLine);
        m_StackCommNb++;
        Activate();
        return TRUE;
    }
    else {
        S32 l_StackIdx = 0;
        S32 l_Size = m_FileStack.GetSize();
        if (l_Size) {
            l_StackIdx = m_FileStack[l_Size - 1].m_StackIndex;
        }
        S32 i = m_StackCommNb;
        while (i > l_StackIdx) {
            m_StackComm[i] = m_StackComm[i - 1];
            i--;
        }
        strcpy(m_StackComm[i].m_CommandLine, i_CommandLine);
        m_StackComm[i].m_CommandName = Name_Z::GetID(i_CommandLine);
        m_StackCommNb++;
        Activate();
        return TRUE;
    }
}

Bool ConsoleInterp_Z::PopCommand(String_Z<CONSOLE_STACK_COMMAND_LEN_MAX>& o_CommandString) {
    S32 l_StackCommNb = m_StackCommNb;
    if (l_StackCommNb == 0) {
        return FALSE;
    }

    S32 l_StackIdx = 0;
    S32 l_FileStackSize = m_FileStack.GetSize();
    if (l_FileStackSize) {
        l_StackIdx = m_FileStack[l_FileStackSize - 1].m_StackIndex;
    }
    if (l_StackCommNb <= l_StackIdx) {
        return FALSE;
    }

    strcpy(o_CommandString, m_StackComm[m_StackCommNb-- - 1].m_CommandLine);
    return TRUE;
}

// TODO: Finish matching
Bool FileInterp_Z::ReplaceParams(Char* i_CommandString, String_Z<CONSOLE_STATIC_COMMAND_LEN_MAX>& o_ReplacedCommandString) {
    String_Z<8> l_ParamNumTemp;
    Char* l_Dest;
    Char* l_ReplaceStart;
    S32 l_Length;

    l_ReplaceStart = strstr(i_CommandString, "%");

    if (!l_ReplaceStart) {
        return FALSE;
    }

    while (l_ReplaceStart) {
        S32 l_StrLen = o_ReplacedCommandString.StrLen();
        l_Dest = &o_ReplacedCommandString[l_StrLen];
        l_Length = (S32)(l_ReplaceStart - i_CommandString);
        strncpy(l_Dest, i_CommandString, l_Length);
        o_ReplacedCommandString[l_StrLen + l_Length] = '\0';

        l_ReplaceStart++;

        l_ParamNumTemp.Empty();
        while (*l_ReplaceStart != '\0') {
            Char l_ReplaceDigit = *l_ReplaceStart;
            if ((l_ReplaceDigit < '0') || (l_ReplaceDigit > '9')) break;

            Char* l_OrigReplaceStart = l_ReplaceStart;
            l_ReplaceStart++;
            l_Length = strlen(l_ParamNumTemp);
            strncpy(&l_ParamNumTemp[l_Length], l_OrigReplaceStart, 1);
            l_ParamNumTemp[l_Length + 1] = '\0';
        }

        l_Length = strlen(l_ParamNumTemp);
        if (l_Length) {
            S32 l_ParamNum = atof(l_ParamNumTemp);
            if ((l_ParamNum >= 1) && (l_ParamNum <= m_ParamCount)) {
                S32 l_ParamLen = m_Params[l_ParamNum - 1].StrLen();
                l_Length = o_ReplacedCommandString.StrLen();
                strncpy(&o_ReplacedCommandString[l_Length], m_Params[l_ParamNum - 1], l_ParamLen);
                o_ReplacedCommandString[l_Length + l_ParamLen] = '\0';

                i_CommandString = l_ReplaceStart;
            }
        }
        else {
            return FALSE;
        }

        l_ReplaceStart = strstr(i_CommandString, "%");
    }

    l_Length = strlen(i_CommandString);
    l_Dest = &o_ReplacedCommandString[o_ReplacedCommandString.StrLen()];
    strncpy(l_Dest, i_CommandString, l_Length);
    l_Dest[l_Length] = '\0';

    return TRUE;
}

void ConsoleInterp_Z::End() {
    if (!m_FileStack.GetSize() && !m_StackCommNb) {
        Deactivate();
    }
}
