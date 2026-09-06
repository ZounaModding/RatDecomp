#include "MenuParser.h"
#include "Color_Z.h"
#include "Console_Z.h"
#include "Name_Z.h"
#include "Program_Z.h"
#include <stdlib.h>
#include <string.h>

extern Name_Z BoxNames[34];
extern const Color COLOR_WHITE;

U32 menuPlafeforme;

Bool MENUPlatform() {
    if (gData.Cons->GetNbParam() == 1) {
        menuPlafeforme = 0xff;
    }
    else {
        S32 l_Count = gData.Cons->GetNbParam();
        menuPlafeforme = 0;
        for (S32 l_Index = 0; l_Index < l_Count - 1; ++l_Index) {
            const Char* l_Platform = gData.Cons->GetParamStr(l_Index + 1);
            if (!stricmp(l_Platform, "PC")) {
                menuPlafeforme |= 1;
            }
            else if (!stricmp(l_Platform, "PS2")) {
                menuPlafeforme |= 2;
            }
            else if (!stricmp(l_Platform, "GC")) {
                menuPlafeforme |= 4;
            }
            else if (!stricmp(l_Platform, "REVO")) {
                menuPlafeforme |= 0x10;
            }
            else if (!stricmp(l_Platform, "MAC")) {
                menuPlafeforme |= 0x20;
            }
            else if (!stricmp(l_Platform, "XBOX")) {
                menuPlafeforme |= 0x40;
            }
            else if (!stricmp(l_Platform, "INTERFACE_DEMO_PS2")) {
                menuPlafeforme |= 0x80;
            }
        }
    }
    return TRUE;
}

S32 GetBoxFromtText(Char* i_Text) {
    Name_Z l_Name = Name_Z(i_Text);
    for (S32 l_Index = 0; l_Index < 34; ++l_Index) {
        const Name_Z& l_BoxName = BoxNames[l_Index];
        if (l_Name == l_BoxName) {
            return l_Index;
        }
    }
    return 0;
}

void GetColorFromText(Color& o_Color, Char* i_Text) {
    o_Color = COLOR_WHITE;
    o_Color.r = atof(std::strstr(i_Text, "r") + 1);
    o_Color.g = atof(std::strstr(i_Text, "g") + 1);
    o_Color.b = atof(std::strstr(i_Text, "b") + 1);
    o_Color.a = atof(std::strstr(i_Text, "a") + 1);
    o_Color *= 1.0f / 512.0f;
    o_Color.a /= 255.0f;
}
