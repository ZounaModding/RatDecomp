#include "Assert_Z.h"
#include "Types_Z.h"
#include "GXStruct.h"
#include "os.h"
#include <string.h>

Extern_Z "C" int sprintf(char* s, const char* format, ...);
#ifdef NONMATCHING_Z
Char ConTemp1[255];
Char ConTemp2[255];
Char ConTemp3[255];
Char ConTemp4[255];
Char ConTemp5[255];
#endif
Bool ExceptionBool_Z(Bool i_Cancelable, const Char* i_Message, ...) {
}

void ExceptionFonc_Z(const Char* a1, const Char* a2, S32 a3, const Char* a4, U32 a5, U32 a6, U32 a7, U32 a8, U32 a9, U64* a10) {
#ifdef NONMATCHING_Z
    Extern_Z Char WhereAmI[0x1C];
    Char l_Buffer[0x28F0];
    GXColor l_FgColor;
    l_FgColor.r = 255;
    l_FgColor.g = 255;
    l_FgColor.b = 255;
    l_FgColor.a = 0;
    GXColor l_BgColor;
    l_BgColor.r = 0;
    l_BgColor.g = 0;
    l_BgColor.b = 255;
    l_BgColor.a = 255;
    sprintf(l_Buffer, "Excep %s %s \"%s\" line%d.\n ", a1, a4, a2, a3);
    strcat(l_Buffer, WhereAmI);
    strcat(l_Buffer, "\n");
    strcat(l_Buffer, "Console:\n");
    strcat(l_Buffer, ConTemp1); // $VIOLET: These \n are only here so I can hack something together in InterpCommand, remove later. Except the one after WhereAmI.
    strcat(l_Buffer, "\n");
    strcat(l_Buffer, ConTemp2);
    strcat(l_Buffer, "\n");
    strcat(l_Buffer, ConTemp3);
    strcat(l_Buffer, "\n");
    strcat(l_Buffer, ConTemp4);
    strcat(l_Buffer, "\n");
    strcat(l_Buffer, ConTemp5);
    strcat(l_Buffer, "\n");
    OSFatal(l_FgColor, l_BgColor, l_Buffer);
#endif
}

// $VIOLET: never called unfortunately / never called in GCConsole_Z::Flush.  add ifdef when we get there
void ExceptionReport(const Char* a1) {
#ifdef NONMATCHING_Z
    strncpy(ConTemp1, ConTemp2, sizeof(ConTemp1));
    strncpy(ConTemp2, ConTemp3, sizeof(ConTemp2));
    strncpy(ConTemp3, ConTemp4, sizeof(ConTemp3));
    strncpy(ConTemp4, ConTemp5, sizeof(ConTemp4));
    strncpy(ConTemp5, a1, sizeof(ConTemp5));
#endif
}

#ifdef NONMATCHING_Z
void ErrorHandler(OSError error, OSContext* context, ...) {
    Extern_Z Char WhereAmI[0x1C];
    Char l_Buffer[0x28F0];
    GXColor l_FgColor;
    l_FgColor.r = 255;
    l_FgColor.g = 255;
    l_FgColor.b = 255;
    l_FgColor.a = 0;
    GXColor l_BgColor;
    l_BgColor.r = 0;
    l_BgColor.g = 0;
    l_BgColor.b = 255;
    l_BgColor.a = 255;

    strcpy(l_Buffer, "ErrorHandler");
    strcat(l_Buffer, WhereAmI);
    strcat(l_Buffer, "\n");
    strcat(l_Buffer, "Console:\n");
    strcat(l_Buffer, ConTemp1);
    strcat(l_Buffer, ConTemp2);
    strcat(l_Buffer, ConTemp3);
    strcat(l_Buffer, ConTemp4);
    strcat(l_Buffer, ConTemp5);

    OSFatal(l_FgColor, l_BgColor, l_Buffer);
}
#endif

void ExceptionHandler() {
#ifdef NONMATCHING_Z
    OSSetErrorHandler(OS_ERROR_DSI, ErrorHandler);
#endif
}

void BreakPoint_Z() {
}
