#include "Console_Z.h"
#include "Memory_Z.h"
#include "Name_Z.h"
#include "ConsoleInterp_Z.h"
#include "Types_Z.h"
#include "String_Z.h"
Extern_Z "C" int strncmp(const char* str1, const char* str2, int n);
Extern_Z "C" size_t strlen(const char* str);
Extern_Z "C" Float atof(const char* str);
Extern_Z "C" int stricmp(const char* String1, const char* String2);

void Console_Z::DisplayHelp() { }

void Console_Z::AddCommand(const Char* i_Command, CommandProc i_Proc, const Char* i_Desc) {
    Command_Z* l_Command = NewL_Z(26) Command_Z;
    if (l_Command) {
        l_Command->m_Command = 0;
        l_Command->m_Alias = 0;
        l_Command->m_Prev = NULL;
        l_Command->m_Proc = NULL;
    }

    l_Command->m_Command = i_Command ? Name_Z::GetID(i_Command, 0) : 0;
    l_Command->m_Proc = i_Proc;

    Char l_Alias[16];
    S32 l_AliasSize = 0;

    S32 len = strlen(i_Command);

    Char* l_AliasStr = l_Alias;

    Char l_Char;
    for (S32 i = 0; i < len && l_AliasSize < 15; i++) {
        l_Char = i_Command[i];
        if ('A' <= l_Char && l_Char <= 'Z' || '0' <= l_Char && l_Char <= '9') {
            *l_AliasStr++ = l_Char;
            l_AliasSize++;
        }
    }

    l_Alias[l_AliasSize] = '\0';
    l_Command->m_Alias = Name_Z::GetID(l_Alias, 0);

    Command_Z* l_Tail = m_CommandList;
    Command_Z* l_Tail2 = (Command_Z*)(void*)l_Tail;

    for (Command_Z* l_TailCopy = m_CommandList; l_TailCopy != NULL; l_TailCopy = l_TailCopy->m_Prev);

    if (NULL == l_Tail) {
        m_CommandList = l_Command;
    }
    else if (l_Tail == l_Tail2) {
        l_Command->m_Prev = l_Tail;
        m_CommandList = l_Command;
    }
    else {
        l_Command->m_Prev = l_Tail->m_Prev;
        l_Tail->m_Prev = l_Command;
    }
}

Bool Console_Z::LaunchCommand(const Char* a1, const Char* i_CommandStr, U32 i_Depth, Command_Z* o_Command) {
    U32 l_Alias;
    if (o_Command == NULL) {
        if (i_CommandStr)
            l_Alias = Name_Z::GetID(i_CommandStr, 0);
        else
            l_Alias = 0;

        o_Command = m_CommandList;
        while ((o_Command != NULL)) {
            if ((l_Alias == o_Command->m_Command) || (l_Alias == o_Command->m_Alias))
                break;
            o_Command = o_Command->m_Prev;
        }
    }

    if (o_Command == NULL)
        return FALSE;

    S32 l_Depth = m_Depth;
    m_Depth = i_Depth;
    Bool l_Result = o_Command->m_Proc();
    m_UnkBool_0x6c70 = FALSE;
    m_Depth = l_Depth;
    return !!l_Result;
    FIXDEBUGINLINING_Z();
}

void Console_Z::NewCommand(const Char* i_CommandStr, U32 i_Depth) {
    InterpCommandLine(i_CommandStr, i_Depth);
}

void Console_Z::PushCommand(const Char* i_CommandLine, Bool i_Unk) {
    m_Interp->PushCommand(i_CommandLine, i_Unk);
}

S32 Console_Z::NbPushedCommand() {
    return m_Interp->NbCommands();
}

Command_Z* PrevCommand;

Command_Z* Console_Z::IsCommand(const Name_Z& i_CommandName) const {
    Command_Z* l_Curr = PrevCommand;

    if (l_Curr != NULL && (i_CommandName == l_Curr->m_Command || i_CommandName == l_Curr->m_Alias)) {
        return l_Curr;
    }

    for (l_Curr = m_CommandList; l_Curr != NULL; l_Curr = l_Curr->m_Prev) {
        if (i_CommandName == l_Curr->m_Command || i_CommandName == l_Curr->m_Alias) {
            PrevCommand = l_Curr;
            return l_Curr;
        }
    }

    return NULL;
}

Bool Console_Z::InterpCommand(const Char* i_CommandStr, U32 i_Depth) {
#ifdef NONMATCHING_Z
    ExceptionReport(i_CommandStr);
#endif
    String_Z<1024> l_CommandStack[16];

    if (strlen(i_CommandStr) == FALSE) {
        return FALSE;
    }

    S32 i;
    S32 l_CommandNameLen;
    const Char* l_CommandStr = i_CommandStr;

    m_NbParam = 0;
    for (i = 0; *l_CommandStr == ' ' || *l_CommandStr == '\t'; i++, l_CommandStr++);

    while (m_NbParam < 16 && i_CommandStr[i]) {
        l_CommandNameLen = 0;

        while (i_CommandStr[i + l_CommandNameLen] != ' '
               && i_CommandStr[i + l_CommandNameLen] != '\0'
               && i_CommandStr[i + l_CommandNameLen] != '\t') {

            S32 l_StrNCmp = strncmp(&i_CommandStr[i + l_CommandNameLen], "\\\"", 2); // $VIOLET: sizeof("\\\"") == 3 so either C++ is bugged.
            if (l_StrNCmp != 0 && i_CommandStr[i + l_CommandNameLen] == '"') {
                i++;
                for (; i_CommandStr[i + l_CommandNameLen] != '"'; l_CommandNameLen++) {
                    if (strncmp(&i_CommandStr[i + l_CommandNameLen], "\\\"", 2) == FALSE) {
                        i++;
                    }
                    l_CommandStack[m_NbParam][l_CommandNameLen] = i_CommandStr[i + l_CommandNameLen];
                }
                i++;
            }
            else {
                if (l_StrNCmp == 0) {
                    i++;
                }
                l_CommandStack[m_NbParam][l_CommandNameLen] = i_CommandStr[i + l_CommandNameLen];
                l_CommandNameLen++;
            }
        }
        l_CommandStack[m_NbParam][l_CommandNameLen] = 0;
        m_StrParam[m_NbParam] = l_CommandStack[m_NbParam];

        Float l_FloatValue;
        if (l_CommandNameLen != 0 && InterpFloat(l_CommandStack[m_NbParam], l_FloatValue) != 0) {
            m_IsFloatParam[m_NbParam] = TRUE;
            m_FloatParam[m_NbParam] = l_FloatValue;
        }
        else {
            m_IsFloatParam[m_NbParam] = FALSE;
        }

        if ((fstricmp(l_CommandStack[m_NbParam], "TRUE") == FALSE)
            || (fstricmp(l_CommandStack[m_NbParam], "On") == FALSE)) {

            Float l_TrueValue = 1.0f;
            m_IsFloatParam[m_NbParam] = TRUE;
            m_FloatParam[m_NbParam] = l_TrueValue;
        }

        if ((fstricmp(l_CommandStack[m_NbParam], "FALSE") == FALSE)
            || (fstricmp(l_CommandStack[m_NbParam], "Off") == FALSE)) {

            Float l_FalseValue = 0.0f;
            m_IsFloatParam[m_NbParam] = TRUE;
            m_FloatParam[m_NbParam] = l_FalseValue;
        }

        i = i + l_CommandNameLen;
        for (l_CommandStr = &i_CommandStr[i]; *l_CommandStr == ' ' || *l_CommandStr == '\t'; i++, l_CommandStr++);
        m_NbParam++;
    }
    S32 l_NbParam = m_NbParam;
    if (l_NbParam == 0) {
        return TRUE;
    }

    Name_Z l_CommandName = Name_Z::GetID(l_CommandStack[0], 0);
    Command_Z* l_Curr = IsCommand(l_CommandName);

    if (l_Curr == NULL) {
        S32 l_NbParam = m_NbParam;

        if (l_NbParam == 3) {
            if (fstricmp(l_CommandStack[0], "#define") == FALSE) {
                m_NbParam--;
                for (i = 0; i < m_NbParam; i++) {
                    l_CommandStack[i].StrCpy(l_CommandStack[i + 1]);
                    m_FloatParam[i] = m_FloatParam[i + 1];
                    m_IsFloatParam[i] = m_IsFloatParam[i + 1];
                }
            }
        }
        else if (l_NbParam <= 2) {
            if (l_NbParam == 2) {
                if (fstricmp(l_CommandStack[0], "#SET") == FALSE) {
                    SetVar(l_CommandStack[1]);
                    return TRUE;
                }
                if (fstricmp(l_CommandStack[0], "#UNSET") == FALSE) {
                    UnVar(l_CommandStack[1]);
                    return TRUE;
                }
                if ((fstricmp(l_CommandStack[0], "#IF") == FALSE)) {
                    PushVar(IsVar(l_CommandStack[1]));
                    return TRUE;
                }
                if ((fstricmp(l_CommandStack[0], "#IFDEF") == FALSE)) {
                    PushVar(IsVar(l_CommandStack[1]));
                    return TRUE;
                }
                if ((fstricmp(l_CommandStack[0], "#IFNOT") == FALSE)) {
                    PushVar(!IsVar(l_CommandStack[1]));
                    return TRUE;
                }
                if ((fstricmp(l_CommandStack[0], "#IFNDEF") == FALSE)) {
                    PushVar(!IsVar(l_CommandStack[1]));
                    return TRUE;
                }
            }

            if (fstricmp(l_CommandStack[0], "#ELSE") == FALSE) {
                ChangeVarState();
                return TRUE;
            }
            if (fstricmp(l_CommandStack[0], "#ENDIF") == FALSE) {
                PopVar();
                return TRUE;
            }
        }
    }

    if (m_StackNbVar != 0 && m_StackVarState[m_StackNbVar - 1] == 0) {
        return TRUE;
    }
    else {
        LaunchCommand(i_CommandStr, l_CommandStack[0], i_Depth, l_Curr);
    }
}

Bool Console_Z::InterpCommandLine(const Char* i_CommandStr, U32 i_Depth) {
    U32 l_Pos = 0;
    S32 l_CurPos = 0;
    U32 l_UnkCount = 0;

    String_Z<1024 + 64> l_CurCommand;

    while (i_CommandStr[l_Pos + l_CurPos] != 0) {
        if (i_CommandStr[l_Pos + l_CurPos] == '\"') {
            if (l_Pos + l_CurPos) {
                if (i_CommandStr[l_Pos + l_CurPos - 1] != '\\') {
                    l_UnkCount++;
                }
            }
        }

        if (l_UnkCount % 2 == 0 && i_CommandStr[l_Pos + l_CurPos] == ';') {
            l_CurCommand.StrnCpy(i_CommandStr + l_Pos, l_CurPos);
            l_CurCommand[l_CurPos] = 0;

            InterpCommand(l_CurCommand, i_Depth);

            l_Pos += l_CurPos;
            l_Pos++;
            l_CurPos = 0;
        }
        else {
            l_CurPos++;
        }
    }

    if (l_CurPos > 0) {
        l_CurCommand.StrnCpy(i_CommandStr + l_Pos, l_CurPos);
        l_CurCommand[l_CurPos] = 0;

        return InterpCommand(l_CurCommand, i_Depth);
    }
    return TRUE;
}

Bool Console_Z::InterpFloat(const Char* i_CommandStr, Float& o_Value) {
    U32 i = 0;
    if (i_CommandStr[0] == 0) {
        return FALSE;
    }
    if (i_CommandStr[0] == '-') {
        i++;
    }

    U8 l_AfterDecimalPoint = FALSE;
    Float l_MultVal = 0.1f;
    Float l_Result = 0.0f;
    while (i_CommandStr[i] != 0) {
        if (i_CommandStr[i] >= '0' && i_CommandStr[i] <= '9') {
            if (l_AfterDecimalPoint) {
                l_Result = l_Result + (l_MultVal * ((i_CommandStr[i]) - '0'));
                l_MultVal *= 0.1f;
            }
            else {
                l_Result *= 10.0f;
                l_Result += ((i_CommandStr[i]) - '0');
            }
        }
        else {
            if (i_CommandStr[i] == '.') {
                if (l_AfterDecimalPoint) {
                    return 0;
                }
                l_AfterDecimalPoint = TRUE;
            }
            else if (i_CommandStr[i] == 'f') {
                return l_AfterDecimalPoint;
            }
            else {
                return FALSE;
            }
        }
        i++;
    }

#ifdef BUGFIXES_Z
    if (l_IsNegative) {
        l_Result = -l_Result;
    }
#endif

    o_Value = l_Result;
    return TRUE;
}

Bool Console_Z::IsVar(const Char* i_Var) {
    for (S32 i = 0; i < m_CommandNbVar; i++) {
        if (stricmp(m_CommandVar[i], i_Var) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void Console_Z::UnVar(const Char* i_Var) {
    if ((m_StackNbVar != 0) && (m_StackVarState[m_StackNbVar - 1]) == FALSE) {
        return;
    }
    for (S32 i = 0; i < m_CommandNbVar; i++) {
        if (stricmp(m_CommandVar[i], i_Var) == 0) {
            m_CommandNbVar--;
            for (; i < m_CommandNbVar; i++) {
                strcpy(m_CommandVar[i], m_CommandVar[i + 1]);
            }
            return;
        }
    }
}

void Console_Z::SetVar(const Char* i_Var) {
    if (IsVar(i_Var)) {
        return;
    }

    if (m_StackNbVar == 0 || m_StackVarState[m_StackNbVar - 1]) {
        strcpy(m_CommandVar[m_CommandNbVar++], i_Var);
    }
}

void Console_Z::PushVar(Bool i_State) {
    if (m_StackNbVar && !m_StackVarState[m_StackNbVar - 1]) {
        i_State = FALSE;
    }

    m_StackVarState[m_StackNbVar++] = i_State;
}

void Console_Z::ChangeVarState() {
    if (m_StackNbVar >= 2 && !m_StackVarState[m_StackNbVar - 2]) {
        return;
    }

    m_StackVarState[m_StackNbVar - 1] = !m_StackVarState[m_StackNbVar - 1];
}
