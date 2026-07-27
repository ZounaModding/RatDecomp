#include "Console_Z.h"
#include "Program_Z.h"
#include <stdio.h>
#include <string.h>

void Console_Z::PrintBoxString(const Char* i_Title, const Char* i_Format, ...) {
    Char l_Buffer[1024];

    if (!(gData.m_EngineFlag & FL_DISABLE_CONSOLE)) {
        va_list l_Args;
        va_start(l_Args, i_Format);
        vsprintf(l_Buffer, i_Format, l_Args);
        va_end(l_Args);
        strcat(l_Buffer, "\n");
        Report_Z(i_Title, l_Buffer);
    }
}

void Console_Z::PrintErrorString(U64 i_ErrorCode, const Char* i_Format, ...) {
}

void Console_Z::MessageError(Bool a1, U64 a2, const Char* a3, va_list& i_Args) {
}

void Console_Z::MessageStatus(U32 a1, const Char* a2, ...) {
}
