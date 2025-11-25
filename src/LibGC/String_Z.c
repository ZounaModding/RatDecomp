#include "__va_arg.h"
#include "Types_Z.h"

Extern_Z "C" S32 vsprintf(const Char* i_Buf, const Char* i_Format, va_list i_Args);

S32 Vsprintf(Char* i_Buf, const Char* i_Format, Char* i_Args) {
    // $VIOLET: might not be va_list. idk man
    va_list l_Unk;
    l_Unk[0].input_arg_area = i_Args;
    l_Unk[0].reg_save_area = i_Args;
    l_Unk[0].gpr = 0;
    l_Unk[0].fpr = 0;
    l_Unk[0].reserved[0] = 0;
    l_Unk[0].reserved[1] = 0;

    vsprintf(i_Buf, i_Format, l_Unk);
    return 0;
}
