#ifndef _LANGUAGE_Z_H_
#define _LANGUAGE_Z_H_
#include "Types_Z.h"
#include "Name_Z.h"
#include "DynArray_Z.h"
U8 GetCharIdFromText(Char* i_Text);
Bool TT(int i_Id);

struct Language_Z {
    /* $VIOLET: These might be defined in the cpp and not in this struct.*/
    static DynArray_Z<Char, 256> LangTextTxt;
    static int LangForMpeg; //$VIOLET: TODO: Make enum def.
    static Char CurChar;
};
#endif
