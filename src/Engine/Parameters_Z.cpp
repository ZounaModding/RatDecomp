#include "Parameters_Z.h"
#include "Program_Z.h"
#include <String_Z.h>
#include "Console_Z.h"

Float ParameterValues[0x4000];
S32 ParametersLoaded = -1;

void AddParameters(int i_Index, Float i_Value) {
    if (ParametersLoaded == -1) {
        return;
    }
    if (i_Index < 0) {
        return;
    }
    if (i_Index >= 0x4000) {
        return;
    }
    ParameterValues[i_Index] = i_Value;
}

void ReadParameters(int i_Index) {
    if (i_Index < 0) {
        FreeParameters();
        return;
    }
    ParametersLoaded = i_Index;
    for (int i = 0; i < 0x4000; i++) {
        ParameterValues[i] = 0.0f;
    }
    String_Z<ARRAY_CHAR_MAX> l_Str;
    l_Str.Sprintf("BSOURCE PARAM\\PP%03d.TSC", i_Index);
    gData.Cons->InterpCommand(l_Str, 0);
}

void FreeParameters() {
    ParametersLoaded = -1;
}
